#include <stdexcept>
#include <vector>

#include "xy.hpp"
#include "matrix.hpp"
#include "block.hpp"

namespace pic2ascii {

const float kLineThickness = 0.1;  // 1.0 is the width of Block

template <typename T>
float sq(T f) {
  return f * f;
}

Block::Block(unsigned int width)
    : width_(width),
      height_(width * 2),
      filter_size_(width / 4),
      has_filtered_cache_(false),
      has_sq_filtered_cache_(false),
      pixels_(width, width * 2),
      filtered_pixels_(width, width * 2),
      sq_filtered_pixels_(width, width * 2) {
  if (width_ <= 0) {
    throw std::logic_error("width should be greater than 0");
  }
  if (filter_size_ % 2 == 0) {
    filter_size_ += 1;  // should be odd number
  }
};

unsigned int& Block::Get(const XY& xy) {
#ifndef PIC2ASCII_RELEASE
  if (IsOutOfRange(xy)) {
    throw std::runtime_error("out of range");
  }
#endif

  return pixels_[xy.x + xy.y * width_];
}

void Block::Set(const XY& xy, unsigned int grayscale) {
#ifndef PIC2ASCII_RELEASE
  if (IsOutOfRange(xy)) {
    throw std::runtime_error("out of range");
  }
#endif

  has_filtered_cache_ = false;
  has_sq_filtered_cache_ = false;
  pixels_[xy.x + xy.y * width_] = grayscale;
}

bool Block::IsOutOfRange(const XY& xy) {
  if (width_ <= xy.x || height_ <= xy.y) {
    return true;
  }
  return false;
}

void Block::Clear() {
  for (unsigned int h = 0u; h < height_; ++h) {
    for (unsigned int w = 0u; w < width_; ++w) {
      Set({w, h}, 255);
    }
  }
}

void Block::Draw(Character ch) {
  Block::Clear();

  switch (ch) {
    case BACKSLASH:
      Block::Line(0.125, 2.0 - 0.125, 1 - 0.125, 0.125);
      break;
    case DASH:
      Block::Line(0.125, 1.0, 1 - 0.125, 1.0);
      break;
    case PIPE:
      Block::Line(0.5, 0.1, 0.5, 1.9);
      break;
    case SLASH:
      Block::Line(0.125, 0.125, 1 - 0.125, 2 - 0.125);
      break;
    case SPACE:
      break;
    default:
      throw std::logic_error("not implemented");
  }
}

void Block::Line(float x1, float y1, float x2, float y2) {
  // line1 (a1x+b1y+c1=0) is the line to draw
  const float a1 = y2 - y1;
  const float b1 = x1 - x2;
  const float c1 = x2 * y1 - x1 * y2;

  for (unsigned int x_ = 0; x_ < width_; ++x_) {
    float x = static_cast<float>(x_) / width_;

    for (unsigned int y_ = 0; y_ < height_; ++y_) {
      float y = 2 - 2 * static_cast<float>(y_) / height_;
      float d_sq =
          ((a1 * a1 * x * x) + (b1 * b1 * y * y) + (2 * a1 * b1 * x * y) +
           (2 * a1 * c1 * x) + (2 * b1 * c1 * y) + (c1 * c1)) /
          (a1 * a1 + b1 * b1);

      if (sq(kLineThickness / 2) < d_sq) {
        continue;
      }

      // line2 (a2x+b2y+c2=0) is vertical line
      float a2 = b1;
      float b2 = -1 * a1;
      float c2 = a1 * y - b1 * x;

      // point (p, q) is the intersection of line1 and line2
      float p = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
      float q = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
      if ((p < x1 && p < x2) || (x1 < p && x2 < p) || (q < y1 && q < y2) ||
          (y1 < q && y2 < q)) {
        continue;
      }

      Set({x_, y_}, 0);
    }
  }
}

Matrix* Block::Filter() {
  if (!has_filtered_cache_) {
    MakeFilteredCache();
  }

  return &filtered_pixels_;
}

Matrix* Block::SQFilter() {
  if (!has_sq_filtered_cache_) {
    MakeSQFilteredCache();
  }

  return &sq_filtered_pixels_;
}

void Block::MakeFilteredCache() {
  const auto filter_offset = (filter_size_ - 1) / 2;  // >= 0
  const auto sq_filter_size = sq(filter_size_);

  for (auto w = filter_offset; w < (width_ - filter_offset); ++w) {
    for (auto h = filter_offset; h < (height_ - filter_offset); ++h) {
      // calculate average in the window
      unsigned int filtered_sum = 0u;
      for (auto x = w - filter_offset; x <= w + filter_offset; ++x) {
        for (auto y = h - filter_offset; y <= h + filter_offset; ++y) {
          filtered_sum += Get({x, y});
        }
      }
      filtered_pixels_[{w, h}] = filtered_sum / sq_filter_size;
    }
  }

  has_filtered_cache_ = true;
}

void Block::MakeSQFilteredCache() {
  const auto filter_offset = (filter_size_ - 1) / 2;  // >= 0
  const auto sq_filter_size = sq(filter_size_);

  Matrix sq_pixels(width_, height_);
  for (auto w = 0u; w < width_; ++w) {
    for (auto h = 0u; h < height_; ++h) {
      sq_pixels[{w, h}] = sq(Get({w, h}));
    }
  }

  for (auto w = filter_offset; w < (width_ - filter_offset); ++w) {
    for (auto h = filter_offset; h < (height_ - filter_offset); ++h) {
      // calculate average in the window
      unsigned int sq_filtered_sum = 0u;
      for (auto x = w - filter_offset; x <= w + filter_offset; ++x) {
        for (auto y = h - filter_offset; y <= h + filter_offset; ++y) {
          sq_filtered_sum += sq_pixels[{x, y}];
        }
      }
      sq_filtered_pixels_[{w, h}] = sq_filtered_sum / sq_filter_size;
    }
  }

  has_sq_filtered_cache_ = true;
}

float Block::MSSIM(Block& other) {
  if (width_ != other.width_ || height_ != other.height_) {
    throw std::runtime_error("the size of blocks does not match");
  }

  const float k1 = 0.01;
  const float k2 = 0.03;
  const float c1 = sq(k1 * 255);
  const float c2 = sq(k2 * 255);

  float total = 0.0;
  unsigned int sample = 0;
  auto filter_offset = (filter_size_ - 1) / 2;  // >= 0

  auto maltiplied = Block(width_);
  for (auto w = 0u; w < width_; ++w) {
    for (auto h = 0u; h < height_; ++h) {
      maltiplied.Set({w, h}, Get({w, h}) * other.Get({w, h}));
    }
  }

  auto filtered_this = *Filter();
  auto filtered_other = *other.Filter();
  auto filtered_this_sq = *SQFilter();
  auto filtered_other_sq = *other.SQFilter();
  auto filtered_maltiplied = *maltiplied.Filter();

  for (auto w = filter_offset; w < (width_ - filter_offset); ++w) {
    for (auto h = filter_offset; h < (height_ - filter_offset); ++h) {
      float x = filtered_this[{w, h}];
      float y = filtered_other[{w, h}];

      float mu_x2 = filtered_this_sq[{w, h}];
      float mu_y2 = filtered_other_sq[{w, h}];
      float mu_xy = filtered_maltiplied[{w, h}];

      float sigma_x2 = mu_x2 - sq(x);
      float sigma_y2 = mu_y2 - sq(y);
      float sigma_xy = mu_xy - x * y;

      float ssim = ((2 * x * y + c1) * (2 * sigma_xy + c2)) /
                   ((sq(x) + sq(y) + c1) * (sigma_x2 + sigma_y2 + c2));
      total += ssim;
      ++sample;
    }
  }

  return total / sample;
}

}  // namespace pic2ascii
