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
      pixels_(width, width * 2) {
  if (width_ <= 0) {
    throw std::logic_error("width should be greater than 0");
  }
  if (filter_size_ % 2 == 0) {
    filter_size_ += 1;  // should be odd number
  }
};

unsigned int& Block::operator[](const XY& xy) {
  if (width_ <= xy.x || height_ <= xy.y) {
    throw std::runtime_error("out of range");
  }
  return pixels_[xy.x + xy.y * width_];
}

void Block::Clear() {
  for (unsigned int i = 0; i < width_ * height_; ++i) {
    pixels_[i] = 255;
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

      (*this)[{x_, y_}] = 0;
    }
  }
}

Block Block::Filter() {
  Block block(width_);
  auto filter_offset = (filter_size_ - 1) / 2;  // >= 0

  for (auto w = filter_offset; w < (width_ - filter_offset); ++w) {
    for (auto h = filter_offset; h < (height_ - filter_offset); ++h) {
      // calculate average in the window
      unsigned int sum = 0u;
      for (auto x = w - filter_offset; x <= w + filter_offset; ++x) {
        for (auto y = h - filter_offset; y <= h + filter_offset; ++y) {
          sum += (*this)[{x, y}];
        }
      }
      block[{w, h}] = sum / sq(filter_size_);
    }
  }

  return block;
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

  auto this_sq = Block(width_);
  for (auto w = 0u; w < width_; ++w) {
    for (auto h = 0u; h < height_; ++h) {
      this_sq[{w, h}] = sq((*this)[{w, h}]);
    }
  }
  auto other_sq = Block(width_);
  for (auto w = 0u; w < width_; ++w) {
    for (auto h = 0u; h < height_; ++h) {
      other_sq[{w, h}] = sq(other[{w, h}]);
    }
  }
  auto maltiplied = Block(width_);
  for (auto w = 0u; w < width_; ++w) {
    for (auto h = 0u; h < height_; ++h) {
      maltiplied[{w, h}] = (*this)[{w, h}] * other[{w, h}];
    }
  }
  auto filtered_this = this->Filter();
  auto filtered_other = other.Filter();  // TODO use cached result
  auto filtered_this_sq = this_sq.Filter();
  auto filtered_other_sq = other_sq.Filter();  // TODO use cached result
  auto filtered_maltiplied = maltiplied.Filter();

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
