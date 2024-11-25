#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "block.hpp"
#include "png.hpp"
#include "main.hpp"

namespace pic2ascii {

Args::Args(int argc, char* argv[]) {
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

  this->input_file = "";
  this->output_file = "";
  switch (args.size()) {
    case 2:
      this->output_file = args[1];
    case 1:
      this->input_file = args[0];
      break;
    default:
      std::cerr << "invalid arguments" << std::endl;
      throw std::runtime_error("the size of blocks does not match");
  }

  this->block_width = 16;     // default
  this->transparent = false;  // default
  for (std::string str : flags) {
    auto tuple = split(str, "=");
    auto flagname = get<0>(tuple);
    auto flagvalue = get<1>(tuple);
    if (flagname == "--block_width") {
      this->block_width = atoi(flagvalue.c_str());
    } else if (flagname == "--transparent") {
      this->transparent = true;
    } else {
      std::cerr << "invalid flag" << std::endl;
      throw std::runtime_error("the size of blocks does not match");
    }
  }
}

std::tuple<std::string, std::string> split(std::string& str, std::string& ch) {
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
  auto args = p2a::Args(argc, argv);

  // read and edit image
  auto block = p2a::Block(args.block_width);
  auto chars = p2a::characters(args.block_width);
  p2a::PNG png(args.input_file.c_str());
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

      if (args.output_file != "") {
        block.Draw(max_char);
        png.WriteNthBlock(x, y, block, args.transparent);
      }
    }
    std::cout << std::endl;
  }

  if (args.output_file != "") {
    png.Save(args.output_file.c_str());
  }
}
