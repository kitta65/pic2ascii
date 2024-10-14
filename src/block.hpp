#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

struct XY {
  unsigned int x;
  unsigned int y;
};

enum Character {
  BACKSLASH,
  DASH,
  PIPE,
  SLASH
};

class Block {
 public:
  unsigned int width;
  unsigned int height;
  std::vector<unsigned char>* pixels;

  Block(unsigned int width, unsigned int height, std::vector<unsigned char>* pixels);
  void Draw(Character ch);
  unsigned char& operator[](XY xy);
  void Clear();
  private:
    void Line(float x0, float y0, float x1, float y1);
};

#endif
