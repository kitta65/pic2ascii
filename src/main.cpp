#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "xy.hpp"
#include "matrix.hpp"
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

  input_file_ = "";
  output_file_ = "";
  switch (args.size()) {
    case 2:
      output_file_ = args[1];
    case 1:
      input_file_ = args[0];
      break;
    default:
      std::cerr << "invalid arguments" << std::endl;
      throw std::runtime_error("the size of blocks does not match");
  }

  block_width_ = 16;     // default
  transparent_ = false;  // default
  for (std::string str : flags) {
    auto tuple = split(str, "=");
    auto flagname = get<0>(tuple);
    auto flagvalue = get<1>(tuple);
    if (flagname == "--block_width") {
      block_width_ = atoi(flagvalue.c_str());
    } else if (flagname == "--transparent") {
      transparent_ = true;
    } else {
      std::cerr << "invalid flag" << std::endl;
      throw std::runtime_error("the size of blocks does not match");
    }
  }
}

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

// returning `const char*` is valid here
// https://stackoverflow.com/questions/2579874/what-is-the-lifetime-of-a-string-literal-returned-by-a-function
const char* print(Character ch) {
  switch (ch) {
    case SYMBOL_BACKSLASH:
      return "\\";
    case SYMBOL_DASH:
      return "-";
    case SYMBOL_HASH:
      return "#";
    case SYMBOL_PIPE:
      return "|";
    case SYMBOL_SLASH:
      return "/";
    case SYMBOL_SPACE:
      return " ";
    case SYMBOL_UNDERSCORE:
      return "_";
  }

  throw std::runtime_error("not implemented");
}

}  // namespace pic2ascii

namespace p2a = pic2ascii;

int main(int argc, char* argv[]) {
  auto args = p2a::Args(argc, argv);

  auto block = p2a::Block(args.block_width_);
  auto chars = p2a::characters(args.block_width_);
  auto results = std::vector<p2a::Character>();
  p2a::PNG png(args.input_file_.c_str());

  for (auto y = 0u; y <= png.MaxY(block); ++y) {
    for (auto x = 0u; x <= png.MaxX(block); ++x) {
      float max_mssim = 0;
      auto max_char = p2a::SYMBOL_SPACE;
      auto has_content = png.ReadNthBlock(x, y, block);
      if (has_content) {
        for (auto c : p2a::kAllCharacters) {
          auto mssim = block.MSSIM(chars[c]);
          if (max_mssim < mssim) {
            max_mssim = mssim;
            max_char = c;
          }
        }
      }

      std::cout << print(max_char);
      results.push_back(max_char);
    }
    std::cout << std::endl;
  }

  if (args.output_file_ != "") {
    const auto size = results.size();
    for (auto i = 0u; i < size; ++i) {
      block.Draw(results[i]);
      png.WriteNthBlock(i, block);
    }
    png.Save(args.output_file_.c_str());
  }
}
