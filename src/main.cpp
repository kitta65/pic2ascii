#include <cstdint>
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

}  // namespace pic2ascii

namespace p2a = pic2ascii;

int main(int argc, char* argv[]) {
  auto args = p2a::Args(argc, argv);

  auto block = p2a::Block(args.block_width_);
  auto representative_chars =
      p2a::characters(p2a::kRepresentativeCharacters, args.block_width_);
  auto equal_hash_group_chars =
      p2a::characters(p2a::kEqualHashGroupCharacters, args.block_width_);
  auto m_x_group_chars =
      p2a::characters(p2a::kMXGroupCharacters, args.block_width_);
  auto t_space_group_chars =
      p2a::characters(p2a::kTSpaceGroupCharacters, args.block_width_);
  auto h_group_chars =
      p2a::characters(p2a::kHGroupCharacters, args.block_width_);

  auto results = std::vector<p2a::Character>();
  p2a::PNG png(args.input_file_.c_str());

  for (auto y = 0u; y <= png.MaxY(block); ++y) {
    for (auto x = 0u; x <= png.MaxX(block); ++x) {
      float max_mssim = 0;
      auto max_char = p2a::SYMBOL_SPACE;
      auto has_content = png.ReadNthBlock(x, y, block);

      if (has_content) {
        std::vector<p2a::Block>* candidate_blocks = &representative_chars;
        const p2a::Character* candidate_chars = p2a::kRepresentativeCharacters;
        for (auto i = 0u; i < (*candidate_blocks).size(); ++i) {
          auto mssim = block.MSSIM(representative_chars[i]);
          if (max_mssim < mssim) {
            max_mssim = mssim;
            max_char = candidate_chars[i];
          }
        }

        switch (max_char) {
          case p2a::SYMBOL_EQUAL:
          case p2a::SYMBOL_HASH:
            candidate_blocks = &equal_hash_group_chars;
            candidate_chars = p2a::kEqualHashGroupCharacters;
            break;
          case p2a::ALPHABET_UPPER_M:
          case p2a::ALPHABET_UPPER_X:
            candidate_blocks = &m_x_group_chars;
            candidate_chars = p2a::kMXGroupCharacters;
            break;
          case p2a::ALPHABET_UPPER_T:
          case p2a::SYMBOL_SPACE:
            candidate_blocks = &t_space_group_chars;
            candidate_chars = p2a::kTSpaceGroupCharacters;
            break;
          case p2a::ALPHABET_UPPER_H:
            candidate_blocks = &h_group_chars;
            candidate_chars = p2a::kHGroupCharacters;
            break;
          default:
            throw std::runtime_error("not implemented");
        }
        for (auto i = 0u; i < (*candidate_blocks).size(); ++i) {
          auto mssim = block.MSSIM((*candidate_blocks)[i]);
          if (max_mssim < mssim) {
            max_mssim = mssim;
            max_char = candidate_chars[i];
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
