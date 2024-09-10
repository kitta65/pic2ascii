#include "pixel.hpp"

Pixel::Pixel() {
  this->red_ = 0;
  this->green_ = 0;
  this->blue_ = 0;
  this->alpha_ = 0;
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b,
             unsigned char a) {
  this->red_ = r;
  this->green_ = g;
  this->blue_ = b;
  this->alpha_ = a;
}

void Pixel::operator=(const Pixel& other) {
  this->red_ = other.red_;
  this->green_ = other.green_;
  this->blue_ = other.blue_;
  this->alpha_ = other.alpha_;
}

void Pixel::Clear() {
  this->red_ = 0;
  this->green_ = 0;
  this->blue_ = 255;
  this->alpha_ = 255;
}