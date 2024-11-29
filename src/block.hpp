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
  unsigned int has_filtered_cache_;
  unsigned int has_sq_filtered_cache_;

  Block(unsigned int width);

  // NOTE (0, 0) is top-left
  unsigned int& Get(const XY& xy);
  void Set(const XY& xy, unsigned int grayscale);

  void Clear();
  void Draw(Character ch);
  Matrix<>* Filter();
  Matrix<>* SQFilter();
  float MSSIM(Block& other);  // structural similarity index measure

 private:
  Matrix<> pixels_;  // shouled be accessed by getter / setter
  Matrix<> filtered_pixels_;
  Matrix<> sq_filtered_pixels_;
  void Line(float x1,
            float y1,
            float x2,
            float y2);  // NOTE (0, 0) is bottom-left
  void MakeFilteredCache();
  void MakeSQFilteredCache();
  bool IsOutOfRange(const XY& xy);
};

}  // namespace pic2ascii

#endif
