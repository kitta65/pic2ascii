#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

struct XY {
  unsigned int x;
  unsigned int y;
};

class Block {
 public:
  unsigned int width;
  unsigned int height;
  Pixel* pixels;

  Block(unsigned int width, unsigned int height, Pixel pixels[]);
  Pixel& operator[](XY xy);
  void Clear();
};

#endif