#include <stdexcept>
#include <vector>
#include <iostream>
#include "block.hpp"

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

void Block::Line(float x0, float y0, float x1, float y1) {
  const float d_max = 0.1;
  const float d2_max = d_max * d_max;

  const float a = y1 - y0;
  const float b = x0 - x1;
  const float c = x1 * y0 - x0 * y1;

  for (unsigned int x_ = 0; x_ < this->width; ++x_) {
    float x = static_cast<float>(x_) / this->width;

    for (unsigned int y_ = 0; y_ < this->height; ++y_) {
      float y = 2 - 2 * static_cast<float>(y_) / this->height;

      float d2 = ((a * a * x * x) + (b * b * y * y) + (2 * a * b * x * y) +
                  (2 * a * c * x) + (2 * b * c * y) + (c * c)) /
                 (a * a + b * b);
      if (d2_max < d2) {
        continue;
      }

      float a_ = b;
      float b_ = -1 * a;
      float c_ = a * y - b * x;

      float p = (b * c_ - b_ * c) / (a * b_ - a_ * b);
      float q = (a_ * c - a * c_) / (a * b_ - a_ * b);
      if ((p < x0 && p < x1) || (x0 < p && x1 < p) || (q < y0 && q < y1) ||
          (y0 < q && y1 < q)) {
        continue;
      }

      (*this)[{x_, y_}] = 0;
    }
  }
}
