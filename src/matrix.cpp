#include <vector>
#include <stdexcept>
#include <tuple>
#include "matrix.hpp"

namespace pic2ascii {

Matrix::Matrix(unsigned int width, unsigned int height)
    : width_(width), height_(height), data_(width * height) {}

unsigned int& Matrix::operator[](unsigned int index) {
  if (width_ * height_ <= index) {
    throw std::runtime_error("out of range");
  }
  return data_[index];
}

unsigned int& Matrix::operator[](std::tuple<unsigned int, unsigned int> xy) {
  auto x = get<0>(xy);
  auto y = get<1>(xy);
  return (*this)[x + y * width_];
}

Matrix Matrix::operator*(Matrix& other) {
  if (width_ != other.width_ || height_ != other.height_) {
    throw std::runtime_error("the size of matrix does not match");
  }
  auto multiplied = Matrix(width_, height_);
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      multiplied[{x, y}] = (*this)[{x, y}] * other[{x, y}];
    }
  }
  return multiplied;
}

}  // namespace pic2ascii
