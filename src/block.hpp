#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

namespace pic2ascii {

#define ALL_CHARACTERS { \
  SYMBOL_BACKSLASH, \
  SYMBOL_CARET, \
  SYMBOL_DASH, \
  SYMBOL_EQUAL, \
  SYMBOL_HASH, \
  SYMBOL_LARGER, \
  SYMBOL_PIPE, \
  SYMBOL_PLUS, \
  SYMBOL_RIGHT_SQUARE_BRACKET, \
  SYMBOL_SLASH, \
  SYMBOL_SMALLER, \
  SYMBOL_SPACE, \
  SYMBOL_UNDERSCORE \
}
enum Character ALL_CHARACTERS;
const Character kAllCharacters[] = ALL_CHARACTERS;
#undef ALL_CHARACTERS

class Block {
 public:
  unsigned int width_;
  unsigned int height_;
  unsigned int filter_size_;
  bool has_filtered_cache_;
  bool has_sq_filtered_cache_;

  Block(unsigned int width);

  // NOTE (0, 0) is top-left
  uint8_t& Get(const XY& xy);
  void Set(const XY& xy, uint8_t grayscale);

  void Clear();
  void Draw(Character ch);
  Matrix<uint8_t>* Filter();
  Matrix<uint16_t>* SQFilter();
  float MSSIM(Block& other);  // structural similarity index measure

 private:
  Matrix<uint8_t> pixels_;  // shouled be accessed by getter / setter
  Matrix<uint8_t> filtered_pixels_;
  Matrix<uint16_t> sq_filtered_pixels_;
  void Line(float x1,
            float y1,
            float x2,
            float y2);  // NOTE (0, 0) is bottom-left
  void MakeFilteredCache();
  void MakeSQFilteredCache();
  bool IsOutOfRange(const XY& xy);
};

template <typename T>
float sq(T f);

template <typename T>
void ApplyFilter(Matrix<T>& source, Matrix<T>& dest, unsigned int filter_size);

template <typename T>
float sq(T f) {
  return f * f;
}

template <typename T>
void ApplyFilter(Matrix<T>& source, Matrix<T>& dest, unsigned int filter_size) {
  const auto filter_offset = (filter_size - 1) / 2;  // >= 0
  const auto sq_filter_size = sq(filter_size);

  for (auto w = filter_offset; w < (source.width_ - filter_offset); ++w) {
    for (auto h = filter_offset; h < (source.height_ - filter_offset); ++h) {
      // calculate average in the window
      unsigned int filtered_sum = 0u;
      for (auto x = w - filter_offset; x <= w + filter_offset; ++x) {
        for (auto y = h - filter_offset; y <= h + filter_offset; ++y) {
          filtered_sum += source[{x, y}];
        }
      }
      dest[{w, h}] = filtered_sum / sq_filter_size;
    }
  }
}

}  // namespace pic2ascii

#endif
