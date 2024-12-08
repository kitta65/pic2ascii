#ifndef kitta65_pic2ascii_block_hpp
#define kitta65_pic2ascii_block_hpp

#include <cstdint>
#include <stdexcept>

#include "xy.hpp"
#include "matrix.hpp"

namespace pic2ascii {

// clang-format off
#define TIER1_CHARACTERS \
  SYMBOL_BACKSLASH, \
  SYMBOL_DASH, \
  SYMBOL_MACRON, \
  SYMBOL_PIPE, \
  SYMBOL_SLASH, \
  SYMBOL_SPACE, \
  SYMBOL_UNDERSCORE
#define TIER2_CHARACTERS \
  ALPHABET_LOWER_K, \
  ALPHABET_LOWER_V, \
  ALPHABET_LOWER_W, \
  ALPHABET_LOWER_X, \
  ALPHABET_LOWER_Z, \
  ALPHABET_UPPER_A, \
  ALPHABET_UPPER_E, \
  ALPHABET_UPPER_F, \
  ALPHABET_UPPER_H, \
  ALPHABET_UPPER_K, \
  ALPHABET_UPPER_L, \
  ALPHABET_UPPER_M, \
  ALPHABET_UPPER_N, \
  ALPHABET_UPPER_T, \
  ALPHABET_UPPER_V, \
  ALPHABET_UPPER_W, \
  ALPHABET_UPPER_X, \
  ALPHABET_UPPER_Y, \
  ALPHABET_UPPER_Z, \
  SYMBOL_CARET, \
  SYMBOL_EQUAL, \
  SYMBOL_HASH, \
  SYMBOL_LARGER, \
  SYMBOL_LEFT_SQUARE_BRACKET, \
  SYMBOL_PLUS, \
  SYMBOL_RIGHT_SQUARE_BRACKET, \
  SYMBOL_SMALLER
enum Character {
  TIER1_CHARACTERS,
  TIER2_CHARACTERS,
};
const Character kAllCharacters[] = {
  TIER1_CHARACTERS,
  TIER2_CHARACTERS,
};
const Character kTier1Characters[] = {TIER1_CHARACTERS};
const Character kTier2Characters[] = {TIER2_CHARACTERS};
#undef TIER1_CHARACTERS
#undef TIER2_CHARACTERS
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

// implement template functions
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
