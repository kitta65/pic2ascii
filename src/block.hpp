#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

struct XY {
  unsigned int x;
  unsigned int y;
};

#define ALL_CHARACTERS {BACKSLASH, DASH, PIPE, SLASH}
enum Character ALL_CHARACTERS;
const Character kAllCharacters[] = ALL_CHARACTERS;
#undef ALL_CHARACTERS

class Block {
 public:
  unsigned int width;
  unsigned int height;
  std::vector<unsigned char>* pixels;

  Block(unsigned int width,
        unsigned int height,
        std::vector<unsigned char>* pixels);
  void Draw(Character ch);
  unsigned char& operator[](XY xy);  // NOTE (0, 0) is top-left
  void Clear();
  float MSSIM(Block& other);  // structural similarity index measure

 private:
  void Line(float x1,
            float y1,
            float x2,
            float y2);  // NOTE (0, 0) is bottom-left
};

#endif
