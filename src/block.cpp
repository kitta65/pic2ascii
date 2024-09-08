#include "pixel.hpp"
#include "block.hpp"

Block::Block(unsigned int height, unsigned int width, Pixel pixels[]) {
  this->height = height;
  this->width = width;
  this->pixels = pixels;
};

Pixel& Block::operator[](XY xy) {
  // TODO check invalid access
  unsigned int x = xy.x;
  unsigned int y = xy.y;
  return (this->pixels)[x * this->width + y];
}
