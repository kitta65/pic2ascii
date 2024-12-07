#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

namespace pic2ascii {

// clang-format off
#define REPRESENTATIVE_CHARACTERS \
  ALPHABET_UPPER_H, \
  ALPHABET_UPPER_M, \
  ALPHABET_UPPER_T, \
  ALPHABET_UPPER_X, \
  SYMBOL_EQUAL, \
  SYMBOL_HASH, \
  SYMBOL_SPACE
#define EQUAL_HASH_GROUP_CHARACTERS \
  ALPHABET_LOWER_V, \
  ALPHABET_LOWER_X, \
  ALPHABET_LOWER_Z, \
  ALPHABET_LOWER_W, \
  ALPHABET_UPPER_A, \
  ALPHABET_UPPER_W, \
  SYMBOL_LARGER, \
  SYMBOL_DASH, \
  SYMBOL_SMALLER, \
  SYMBOL_PLUS
#define M_X_GROUP_CHARACTERS \
  ALPHABET_UPPER_V, \
  ALPHABET_UPPER_Y, \
  ALPHABET_UPPER_Z, \
  SYMBOL_BACKSLASH, \
  SYMBOL_SLASH, \
  SYMBOL_UNDERSCORE
#define T_SPACE_GROUP_CHARACTERS \
  SYMBOL_CARET, \
  SYMBOL_MACRON, \
  SYMBOL_LEFT_SQUARE_BRACKET, \
  SYMBOL_PIPE, \
  SYMBOL_RIGHT_SQUARE_BRACKET
#define H_GROUP_CHARACTERS \
  ALPHABET_LOWER_K, \
  ALPHABET_UPPER_E, \
  ALPHABET_UPPER_F, \
  ALPHABET_UPPER_K, \
  ALPHABET_UPPER_L, \
  ALPHABET_UPPER_N
enum Character {
  REPRESENTATIVE_CHARACTERS,
  EQUAL_HASH_GROUP_CHARACTERS,
  M_X_GROUP_CHARACTERS,
  T_SPACE_GROUP_CHARACTERS,
  H_GROUP_CHARACTERS,
};
const Character kAllCharacters[] = {
  REPRESENTATIVE_CHARACTERS,
  EQUAL_HASH_GROUP_CHARACTERS,
  M_X_GROUP_CHARACTERS,
  T_SPACE_GROUP_CHARACTERS,
  H_GROUP_CHARACTERS,
};
const Character kRepresentativeCharacters[] = {REPRESENTATIVE_CHARACTERS};
const Character kEqualHashGroupCharacters[] = {EQUAL_HASH_GROUP_CHARACTERS};
const Character kMXGroupCharacters[] = {M_X_GROUP_CHARACTERS};
const Character kTSpaceGroupCharacters[] = {T_SPACE_GROUP_CHARACTERS};
const Character kHGroupCharacters[] = {H_GROUP_CHARACTERS};
#undef REPERESENTATIVE_CHARACTERS
#undef EQUAL_HASH_GROUP_CHARACTERS
#undef M_X_GROUP_CHARACTERS
#undef T_SPACE_GROUP_CHARACTERS
#undef H_GROUP_CHARACTERS
// clang-format on

const char* print(Character);

class Block {
 public:
  unsigned int width_;
  unsigned int height_;
  unsigned int filter_size_;
  unsigned int skip_size_;
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
T sq(T f);

template <typename T>
void ApplyFilter(Matrix<T>& source,
                 Matrix<T>& dest,
                 unsigned int filter_size,
                 unsigned int skip_size);

template <typename T>
T sq(T f) {
  return f * f;
}

template <typename T>
void ApplyFilter(Matrix<T>& source,
                 Matrix<T>& dest,
                 unsigned int filter_size,
                 unsigned int skip_size) {
  const auto filter_offset = (filter_size - 1) / 2;  // >= 0

  for (auto w = filter_offset; w < (source.width_ - filter_offset);
       w += skip_size) {
    for (auto h = filter_offset; h < (source.height_ - filter_offset);
         h += skip_size) {
      // calculate average in the window
      unsigned int filtered_sum = 0u;
      unsigned int sample = 0u;
      for (auto x = w - filter_offset; x <= w + filter_offset; x += skip_size) {
        for (auto y = h - filter_offset; y <= h + filter_offset;
             y += skip_size) {
          filtered_sum += source[{x, y}];
          ++sample;
        }
      }
      dest[{w, h}] = filtered_sum / sample;
    }
  }
}

}  // namespace pic2ascii

#endif
