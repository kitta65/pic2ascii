#include <vector>
#include <stdexcept>
#include <tuple>
#include "xy.hpp"
#include "matrix.hpp"

namespace pic2ascii {

Matrix::Matrix(unsigned int width, unsigned int height)
    : width_(width), height_(height), data_(width * height) {}

unsigned int& Matrix::operator[](unsigned int index) {
#ifndef PIC2ASCII_RELEASE
  if (width_ * height_ <= index) {
    throw std::runtime_error("out of range");
  }
#endif

  return data_[index];
}

unsigned int& Matrix::operator[](const XY& xy) {
  return (*this)[xy.x + xy.y * width_];
}

Matrix Matrix::operator*(Matrix& other) {
#ifndef PIC2ASCII_RELEASE
  if (width_ != other.width_ || height_ != other.height_) {
    throw std::runtime_error("the size of matrix does not match");
  }
#endif

  auto multiplied = Matrix(width_, height_);
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      multiplied[{x, y}] = (*this)[{x, y}] * other[{x, y}];
    }
  }
  return multiplied;
}

}  // namespace pic2ascii
