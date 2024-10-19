#include <stdexcept>
#include <vector>
#include "block.hpp"

// 1.0 means the width of Block
const float kLineThickness = 0.2;

float sq(float f) {
  return f * f;
}

// TODO since height is always width*2, height argument is not needed
Block::Block(unsigned int width,
             unsigned int height,
             std::vector<unsigned char>* pixels) {
  this->width = width;
  this->height = height;
  this->pixels = pixels;
};

unsigned char& Block::operator[](XY xy) {
  if (this->width <= xy.x || this->height <= xy.y) {
    throw std::runtime_error("out of range");
  }
  unsigned int x = xy.x;
  unsigned int y = xy.y;
  return (*(this->pixels))[x + y * this->width];
}

void Block::Clear() {
  for (unsigned int i = 0; i < this->width * this->height; ++i) {
    (*pixels)[i] = 255;
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
    default:
      throw std::logic_error("not implemented");
  }
}

void Block::Line(float x1, float y1, float x2, float y2) {
  // line1 (a1x+b1y+c1=0) is the line to draw
  const float a1 = y2 - y1;
  const float b1 = x1 - x2;
  const float c1 = x2 * y1 - x1 * y2;

  for (unsigned int x_ = 0; x_ < this->width; ++x_) {
    float x = static_cast<float>(x_) / this->width;

    for (unsigned int y_ = 0; y_ < this->height; ++y_) {
      float y = 2 - 2 * static_cast<float>(y_) / this->height;
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

float Block::MSSIM(Block& other) {
  if (this->width != other.width || this->height != other.height) {
    throw std::runtime_error("the size of blocks does not match");
  }

  const float k1 = 0.01;
  const float k2 = 0.03;
  const float c1 = sq(k1 * 255);
  const float c2 = sq(k2 * 255);

  float total = 0.0;
  unsigned int sample = 0;
  for (unsigned int w = 0; w < this->width; ++w) {
    for (unsigned int h = 0; h < this->height; ++h) {
      // TODO skip edge if needed

      float x = (*this)[{w, h}];
      float y = other[{w, h}];

      float mu_x2 = sq(x);
      float mu_y2 = sq(y);
      float mu_xy = x * y;

      float sigma_x2 = mu_x2 - sq(x);
      float sigma_y2 = mu_y2 - sq(y);
      float sigma_xy = mu_xy - x * y;

      float ssim = ((2 * mu_xy + c1) * (sigma_xy + c2)) /
                   ((mu_x2 + mu_y2 + c1) * (sigma_x2 + sigma_y2 + c2));
      total += ssim;
      ++sample;
    }
  }

  return total / sample;
}
