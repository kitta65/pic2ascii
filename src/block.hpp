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
  std::vector<unsigned char>* pixels;

  Block(unsigned int width, unsigned int height, std::vector<unsigned char>* pixels);
  unsigned char& operator[](XY xy);
  void Clear();
};

#endif
