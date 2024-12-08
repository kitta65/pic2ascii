#ifndef kitta65_pic2ascii_main_hpp
#define kitta65_pic2ascii_main_hpp

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "block.hpp"
#include "png.hpp"

namespace pic2ascii {

class Args {
 public:
  std::string input_file_;
  std::string output_file_;
  unsigned int block_width_;
  bool transparent_;
  Args(int argc, char* argv[]);
};

std::tuple<std::string, std::string> split(std::string str, std::string ch);
template <std::size_t N>
std::vector<Block> characters(const Character (&charset)[N],
                              unsigned int block_width);

// implement template function
template <std::size_t N>
std::vector<Block> characters(const Character (&charset)[N],
                              unsigned int block_width) {
  auto chars = std::vector<Block>();
  for (auto i = 0u; i < N; ++i) {
    auto char_ = Block(block_width);
    char_.Draw(charset[i]);
    chars.push_back(char_);
  }
  return chars;
}

}  // namespace pic2ascii

#endif
