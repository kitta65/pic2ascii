#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

struct XY {
  unsigned int x;
  unsigned int y;
};

class Block {
 private:
  unsigned int height;
  unsigned int width;
  Pixel* pixels;

 public:
  Block(unsigned int height, unsigned int width, Pixel pixels[]);
  Pixel& operator[](XY xy);
  void Clear();
};

#endif