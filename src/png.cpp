#include <stdexcept>
#include <tuple>
#include <vector>

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"
// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

#include "matrix.hpp"
#include "block.hpp"
#include "png.hpp"

namespace pic2ascii {

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

unsigned int PNG::MaxX(const Block& block) {
  auto max_block_x = this->width / block.width_;
  if (this->width % block.width_ == 0) {
    --max_block_x;
  }

  return max_block_x;
}

unsigned int PNG::MaxY(const Block& block) {
  auto max_block_y = this->height / block.height_;
  if (this->height % block.height_ == 0) {
    --max_block_y;
  }
  return max_block_y;
}

bool PNG::ReadNthBlock(unsigned int index, Block& block) {
  auto max_block_x = PNG::MaxX(block);
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width_ + block_y * block.height_ * this->width) *
      kNumChannels;

  if (this->width * this->height * kNumChannels <= base_idx) {
    throw std::runtime_error("out of range");
  }

  auto has_content = false;
  for (auto pixel_y = 0u; pixel_y < block.height_; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width_; ++pixel_x) {
      if (this->width <= block_x * block.width_ + pixel_x ||
          this->height <= block_y * block.height_ + pixel_y) {
        block[{pixel_x, pixel_y}] = 255;
      } else if (this->data[base_idx + 3] <= 127) {  // if transparent
        block[{pixel_x, pixel_y}] = 255;
      } else {
        auto grayscale = 0.299 * this->data[base_idx + offset_idx + 0] +
                         0.587 * this->data[base_idx + offset_idx + 1] +
                         0.114 * this->data[base_idx + offset_idx + 2];
        block[{pixel_x, pixel_y}] = grayscale;
      }

      if (block[{pixel_x, pixel_y}] < 255) {
        has_content = true;
      }
      offset_idx += kNumChannels;
    }
  }
  return has_content;
}

bool PNG::ReadNthBlock(unsigned int x, unsigned int y, Block& block) {
  return PNG::ReadNthBlock(x + (PNG::MaxX(block) + 1) * y, block);
}

void PNG::WriteNthBlock(unsigned int index, Block& block, bool transparent) {
  auto max_block_x = PNG::MaxX(block);
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width_ + block_y * block.height_ * this->width) *
      kNumChannels;

  for (auto pixel_y = 0u; pixel_y < block.height_; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width_; ++pixel_x) {
      if (this->width <= block_x * block.width_ + pixel_x ||
          this->height <= block_y * block.height_ + pixel_y) {
        // NOP
      } else {
        unsigned char grayscale = block[{pixel_x, pixel_y}];
        this->data[base_idx + offset_idx + 0] = grayscale;
        this->data[base_idx + offset_idx + 1] = grayscale;
        this->data[base_idx + offset_idx + 2] = grayscale;
        if (transparent) {
          this->data[base_idx + offset_idx + 3] = grayscale <= 127 ? 255 : 0;
        } else {
          this->data[base_idx + offset_idx + 3] = 255;
        }
      }
      offset_idx += kNumChannels;
    }
  }
}

void PNG::WriteNthBlock(unsigned int x,
                        unsigned int y,
                        Block& block,
                        bool transparent) {
  PNG::WriteNthBlock(x + (PNG::MaxX(block) + 1) * y, block, transparent);
}

void PNG::Save(const char* file) {
  int result = stbi_write_png(file, this->width, this->height, kNumChannels,
                              this->data, this->width * kNumChannels);
  if (result == 0) {
    throw std::runtime_error("failed to write image");
  }
};

}  // namespace pic2ascii
