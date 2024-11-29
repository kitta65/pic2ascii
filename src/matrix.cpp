#include <vector>
#include <stdexcept>
#include <tuple>
#include "matrix.hpp"

namespace pic2ascii {

Matrix::Matrix() {
  this->width = 0;
  this->height = 0;
  this->data = std::vector<unsigned int>(0);
}

Matrix::Matrix(unsigned int width, unsigned int height) {
  this->width = width;
  this->height = height;
  this->data = std::vector<unsigned int>(width * height);
}

unsigned int& Matrix::operator[](unsigned int index) {
  if (this->width * this->height <= index) {
    throw std::runtime_error("out of range");
  }
  return this->data[index];
}

unsigned int& Matrix::operator[](std::tuple<unsigned int, unsigned int> xy) {
  auto x = get<0>(xy);
  auto y = get<1>(xy);
  return (*this)[x + y * this->width];
}

Matrix Matrix::operator*(Matrix& other) {
  if (this->width != other.width || this->height != other.height) {
    throw std::runtime_error("the size of matrix does not match");
  }
  auto multiplied = Matrix(this->width, this->height);
  for (auto y = 0u; y < this->height; ++y) {
    for (auto x = 0u; x < this->width; ++x) {
      multiplied[{x, y}] = (*this)[{x, y}] * other[{x, y}];
    }
  }
  return multiplied;
}

}  // namespace pic2ascii
