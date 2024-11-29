#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

namespace pic2ascii {

#define ALL_CHARACTERS {BACKSLASH, DASH, PIPE, SLASH, SPACE}
enum Character ALL_CHARACTERS;
const Character kAllCharacters[] = ALL_CHARACTERS;
#undef ALL_CHARACTERS

class Block {
 public:
  unsigned int width_;
  unsigned int height_;
  unsigned int filter_size_;

  Block(unsigned int width);
  unsigned int& operator[](const XY& xy);  // NOTE (0, 0) is top-left
  void Clear();
  void Draw(Character ch);
  Block Filter();
  float MSSIM(Block& other);  // structural similarity index measure

 private:
  Matrix pixels_;
  void Line(float x1,
            float y1,
            float x2,
            float y2);  // NOTE (0, 0) is bottom-left
};

}  // namespace pic2ascii

#endif
