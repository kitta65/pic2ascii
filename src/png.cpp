#include "png.hpp"

// NOTE
// include statements below implement functions.
// they are not ordinary header files.

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"
// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

namespace pic2ascii {

const int kNumChannels = 4;

PNG::PNG(const char* file) {
  int width, height, _n;

  unsigned char* data = stbi_load(file, &width, &height, &_n, kNumChannels);
  if (data == NULL) {
    throw std::runtime_error("failed to load image");
  }

  width_ = width;
  height_ = height;
  data_ = data;
};

PNG::~PNG() {
  stbi_image_free(data_);
}

unsigned int PNG::MaxX(const Block& block) {
  auto max_block_x = width_ / block.width_;
  if (width_ % block.width_ == 0) {
    --max_block_x;
  }

  return max_block_x;
}

unsigned int PNG::MaxY(const Block& block) {
  auto max_block_y = height_ / block.height_;
  if (height_ % block.height_ == 0) {
    --max_block_y;
  }
  return max_block_y;
}

bool PNG::ReadNthBlock(unsigned int index, Block& block) {
  auto max_block_x = MaxX(block);
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width_ + block_y * block.height_ * width_) *
      kNumChannels;

  if (width_ * height_ * kNumChannels <= base_idx) {
    throw std::runtime_error("out of range");
  }

  auto has_content = false;
  for (auto pixel_y = 0u; pixel_y < block.height_; ++pixel_y) {
    auto offset_idx = pixel_y * width_ * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width_; ++pixel_x) {
      if (width_ <= block_x * block.width_ + pixel_x ||
          height_ <= block_y * block.height_ + pixel_y) {
        block.Set({pixel_x, pixel_y}, 255);
      } else if (data_[base_idx + 3] <= 127) {  // if transparent
        block.Set({pixel_x, pixel_y}, 255);
      } else {
        auto grayscale = 0.299 * data_[base_idx + offset_idx + 0] +
                         0.587 * data_[base_idx + offset_idx + 1] +
                         0.114 * data_[base_idx + offset_idx + 2];
        block.Set({pixel_x, pixel_y}, grayscale);
      }

      if (block.Get({pixel_x, pixel_y}) < 255) {
        has_content = true;
      }
      offset_idx += kNumChannels;
    }
  }
  return has_content;
}

bool PNG::ReadNthBlock(unsigned int x, unsigned int y, Block& block) {
  return ReadNthBlock(x + (MaxX(block) + 1) * y, block);
}

void PNG::WriteNthBlock(unsigned int index, Block& block, bool transparent) {
  auto max_block_x = MaxX(block);
  const auto block_x = index % (max_block_x + 1);
  const auto block_y = index / (max_block_x + 1);
  const auto base_idx =
      (block_x * block.width_ + block_y * block.height_ * width_) *
      kNumChannels;

  for (auto pixel_y = 0u; pixel_y < block.height_; ++pixel_y) {
    auto offset_idx = pixel_y * width_ * kNumChannels;
    for (auto pixel_x = 0u; pixel_x < block.width_; ++pixel_x) {
      if (width_ <= block_x * block.width_ + pixel_x ||
          height_ <= block_y * block.height_ + pixel_y) {
        // NOP
      } else {
        unsigned char grayscale = block.Get({pixel_x, pixel_y});
        data_[base_idx + offset_idx + 0] = grayscale;
        data_[base_idx + offset_idx + 1] = grayscale;
        data_[base_idx + offset_idx + 2] = grayscale;
        if (transparent) {
          data_[base_idx + offset_idx + 3] = grayscale <= 127 ? 255 : 0;
        } else {
          data_[base_idx + offset_idx + 3] = 255;
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
  WriteNthBlock(x + (MaxX(block) + 1) * y, block, transparent);
}

void PNG::Save(const char* file) {
  int result = stbi_write_png(file, width_, height_, kNumChannels, data_,
                              width_ * kNumChannels);
  if (result == 0) {
    throw std::runtime_error("failed to write image");
  }
};

}  // namespace pic2ascii
