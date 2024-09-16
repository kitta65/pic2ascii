#include "pixel.hpp"
#include "block.hpp"

Block::Block(unsigned int width, unsigned int height, Pixel pixels[]) {
  this->width = width;
  this->height = height;
  this->pixels = pixels;
};

Pixel& Block::operator[](XY xy) {
  // TODO check invalid access
  unsigned int x = xy.x;
  unsigned int y = xy.y;
  return (this->pixels)[x + y * this->width];
}

void Block::Clear() {
  for (unsigned int i = 0; i < this->width * this->height; ++i) {
    Pixel p = pixels[i];
    p.Clear();
  }
}
