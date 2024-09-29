#include <stdexcept>
#include <vector>

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"
// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

#include "pixel.hpp"
#include "block.hpp"
#include "png.hpp"

const int kNumChannels = 4;

PNG::PNG(char* file) {
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

void PNG::Save(char* file) {
  int result = stbi_write_png(file, this->width, this->height, kNumChannels,
                              this->data, this->width * kNumChannels);
  if (result == 0) {
    throw std::runtime_error("failed to write image");
    stbi_image_free(this->data);
  }
};

bool PNG::ReadNthBlock(unsigned int index, Block& block) {
  const auto max_block_x = this->width / block.width;
  const auto block_x = index % max_block_x;
  const auto block_y = index / max_block_x;
  const auto base_pixel_x = block_x * block.width;
  const auto base_pixel_y = block_y * block.height;
  const auto base_idx =
      (base_pixel_x + base_pixel_y * this->width) * kNumChannels;

  if (this->width * this->height * kNumChannels <= base_idx) {
    return false;
  }

  for (auto pixel_y = 0u; pixel_y < block.height; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width; ++pixel_x) {
      block[{pixel_x, pixel_y}] = Pixel(this->data[base_idx + offset_idx + 0],
                                        this->data[base_idx + offset_idx + 1],
                                        this->data[base_idx + offset_idx + 2],
                                        this->data[base_idx + offset_idx + 3]);
      offset_idx += kNumChannels;
    }
  }
  return true;
}

void PNG::WriteNthBlock(unsigned int index, Block& block) {
  const auto max_block_x = this->width / block.width;
  const auto block_x = index % max_block_x;
  const auto block_y = index / max_block_x;

  const auto base_pixel_x = block_x * block.width;
  const auto base_pixel_y = block_y * block.height;
  const auto base_idx =
      (base_pixel_x + base_pixel_y * this->width) * kNumChannels;

  for (auto pixel_y = 0u; pixel_y < block.height; ++pixel_y) {
    auto offset_idx = pixel_y * width * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width; ++pixel_x) {
      Pixel pixel = block[{pixel_x, pixel_y}];
      pixel.Clear();
      this->data[base_idx + offset_idx + 0] = pixel.red;
      this->data[base_idx + offset_idx + 1] = pixel.green;
      this->data[base_idx + offset_idx + 2] = pixel.blue;
      this->data[base_idx + offset_idx + 3] = pixel.alpha;
      offset_idx += kNumChannels;
    }
  }
}
