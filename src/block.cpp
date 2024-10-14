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

void Block::Line(float x0, float y0, float x1, float y1) {
  // line1 (a1x+b1y+c1=0) is the line to draw
  const float a1 = y1 - y0;
  const float b1 = x0 - x1;
  const float c1 = x1 * y0 - x0 * y1;

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
      if ((p < x0 && p < x1) || (x0 < p && x1 < p) || (q < y0 && q < y1) ||
          (y0 < q && y1 < q)) {
        continue;
      }

      (*this)[{x_, y_}] = 0;
    }
  }
}
