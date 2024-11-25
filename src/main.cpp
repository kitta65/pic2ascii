#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "block.hpp"
#include "png.hpp"
#include "main.hpp"

namespace pic2ascii {

std::tuple<std::string, std::string> split(std::string str, std::string ch) {
  auto pos = str.find(ch);
  auto left = str.substr(0, pos);
  auto right = str.substr(pos + 1);
  return std::tuple(left, right);
}

std::vector<Block> characters(unsigned int block_width) {
  auto chars = std::vector<Block>();
  for (auto c : kAllCharacters) {
    auto char_ = Block(block_width);
    char_.Draw(c);
    chars.push_back(char_);
  }
  return chars;
}

}  // namespace pic2ascii

namespace p2a = pic2ascii;

int main(int argc, char* argv[]) {
  // handle CLI arguments
  auto args = std::vector<std::string>();
  auto flags = std::vector<std::string>();
  for (auto i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.starts_with("--")) {
      flags.push_back(arg);
    } else {
      args.push_back(arg);
    }
  }

  const char* input_file = NULL;
  const char* output_file = NULL;
  switch (args.size()) {
    case 2:
      output_file = args[1].c_str();
    case 1:
      input_file = args[0].c_str();
      break;
    default:
      std::cerr << "invalid arguments" << std::endl;
      return EXIT_FAILURE;
  }

  unsigned int block_width = 4;  // default
  auto transparent = false;      // default
  for (std::string str : flags) {
    auto tuple = p2a::split(str, "=");
    auto flagname = get<0>(tuple);
    auto flagvalue = get<1>(tuple);
    if (flagname == "--block_width") {
      block_width = atoi(flagvalue.c_str());
    } else if (flagname == "--transparent") {
      transparent = true;
    } else {
      std::cerr << "invalid flag" << std::endl;
      return EXIT_FAILURE;
    }
  }

  // read and edit image
  auto block = p2a::Block(block_width);
  auto chars = p2a::characters(block_width);
  p2a::PNG png(input_file);
  // (0, y) is scaned twice but don't mind
  for (auto y = 0u; png.ReadNthBlock(0, y, block); ++y) {
    for (auto x = 0u; png.ReadNthBlock(x, y, block); ++x) {
      float max_mssim = 0;
      auto max_char = p2a::PIPE;  // TODO more reasonable default
      for (auto c : p2a::kAllCharacters) {
        auto char_ = chars[c];
        auto mssim = block.MSSIM(char_);
        if (max_mssim < mssim) {
          max_mssim = mssim;
          max_char = c;
        }
      }

      switch (max_char) {
        case p2a::BACKSLASH:
          std::cout << "\\";
          break;
        case p2a::DASH:
          std::cout << "-";
          break;
        case p2a::PIPE:
          std::cout << "|";
          break;
        case p2a::SLASH:
          std::cout << "/";
          break;
        case p2a::SPACE:
          std::cout << " ";
          break;
      }

      if (output_file != NULL) {
        block.Draw(max_char);
        png.WriteNthBlock(x, y, block, transparent);
      }
    }
    std::cout << std::endl;
  }

  if (output_file != NULL) {
    png.Save(output_file);
  }
}
