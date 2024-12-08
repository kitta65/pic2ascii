#ifndef kitta65_pic2ascii_matrix_hpp
#define kitta65_pic2ascii_matrix_hpp

#include <vector>
#include "xy.hpp"

namespace pic2ascii {

template <typename T = unsigned int>
class Matrix {
 public:
  unsigned int width_;
  unsigned int height_;
  std::vector<T> data_;

  Matrix(unsigned int width, unsigned int height);
  T& operator[](unsigned int index);
  T& operator[](const XY& xy);  // NOTE (0, 0) is top-left
  Matrix<T> operator*(Matrix<T>& other);
};

// implement template functions
template <typename T>
Matrix<T>::Matrix(unsigned int width, unsigned int height)
    : width_(width), height_(height), data_(width * height) {}

template <typename T>
T& Matrix<T>::operator[](unsigned int index) {
#ifndef PIC2ASCII_RELEASE
  if (width_ * height_ <= index) {
    throw std::runtime_error("out of range");
  }
#endif

  return data_[index];
}

template <typename T>
T& Matrix<T>::operator[](const XY& xy) {
  return (*this)[xy.x + xy.y * width_];
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix& other) {
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

#endif
