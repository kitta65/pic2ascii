#include "pixel.hpp"

Pixel::Pixel() {
  this->red = 0;
  this->green = 0;
  this->blue = 0;
  this->alpha = 0;
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b,
             unsigned char a) {
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

void Pixel::Clear() {
  this->red = 0u;
  this->green = 0u;
  this->blue = 0u;
  this->alpha = 0u;
}
