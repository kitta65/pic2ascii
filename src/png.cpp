#include <stdexcept>
#include <vector>

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"
// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

#include "block.hpp"
#include "png.hpp"

const int kNumChannels = 4;

PNG::PNG(const char* file) {
  int width, height, _n;

  unsigned char* data = stbi_load(file, &width, &height, &_n, kNumChannels);
  if (data == NULL) {
    throw std::runtime_error("failed to load image");
  }

  this->width = width;
  this->height = height;
  this->data = data;
};

PNG::~PNG() {
  stbi_image_free(this->data);
}

void PNG::Save(const char* file) {
  int result = stbi_write_png(file, this->width, this->height, kNumChannels,
                              this->data, this->width * kNumChannels);
  if (result == 0) {
    throw std::runtime_error("failed to write image");
    stbi_image_free(this->data);
  }
};

bool PNG::ReadNthBlock(unsigned int index, Block& block) {
  auto max_block_x = this->width / block.width;
  if (this->width % block.width == 0) {
    --max_block_x;
  }
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width + block_y * block.height * this->width) *
      kNumChannels;

  if (this->width * this->height * kNumChannels <= base_idx) {
    return false;
  }

  for (auto pixel_y = 0u; pixel_y < block.height; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width; ++pixel_x) {
      if (this->width <= block_x * block.width + pixel_x ||
          this->height <= block_y * block.height + pixel_y) {
        block[{pixel_x, pixel_y}] = 255;
      } else if (this->data[base_idx + 3] <= 127) {  // if transparent
        block[{pixel_x, pixel_y}] = 255;
      } else {
        auto grayscale = 0.299 * this->data[base_idx + offset_idx + 0] +
                         0.587 * this->data[base_idx + offset_idx + 1] +
                         0.114 * this->data[base_idx + offset_idx + 2];
        block[{pixel_x, pixel_y}] = grayscale;
      }
      offset_idx += kNumChannels;
    }
  }
  return true;
}

void PNG::WriteNthBlock(unsigned int index, Block& block) {
  auto max_block_x = this->width / block.width;
  if (this->width % block.width == 0) {
    --max_block_x;
  }
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width + block_y * block.height * this->width) *
      kNumChannels;

  for (auto pixel_y = 0u; pixel_y < block.height; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width; ++pixel_x) {
      if (this->width <= block_x * block.width + pixel_x ||
          this->height <= block_y * block.height + pixel_y) {
        // NOP
      } else {
        unsigned char grayscale = block[{pixel_x, pixel_y}];
        this->data[base_idx + offset_idx + 0] = grayscale;
        this->data[base_idx + offset_idx + 1] = grayscale;
        this->data[base_idx + offset_idx + 2] = grayscale;
        this->data[base_idx + offset_idx + 3] = grayscale <= 127 ? 255 : 0;
      }
      offset_idx += kNumChannels;
    }
  }
}
