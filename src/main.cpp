#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "block.hpp"
#include "png.hpp"

std::tuple<std::string, std::string> split(std::string str, std::string ch);

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
  for (std::string str : flags) {
    auto tuple = split(str, "=");
    auto flagname = get<0>(tuple);
    auto flagvalue = get<1>(tuple);
    if (flagname == "--block_width") {
      block_width = atoi(flagvalue.c_str());
    } else {
      std::cerr << "invalid flag" << std::endl;
      return EXIT_FAILURE;
    }
  }

  // read and edit image
  auto block = Block(block_width);
  PNG png(input_file);
  for (auto idx = 0u; png.ReadNthBlock(idx, block); ++idx) {
    block.Clear();
    if (idx == 0u) {
      block.Draw(PIPE);
    }
    if (idx == 1u) {
      block.Draw(DASH);
    }
    if (idx == 2u) {
      block.Draw(PIPE);
    }
    png.WriteNthBlock(idx, block);
  }
  png.Save(output_file);
}

std::tuple<std::string, std::string> split(std::string str, std::string ch) {
  auto pos = str.find(ch);
  auto left = str.substr(0, pos);
  auto right = str.substr(pos + 1);
  return std::tuple(left, right);
}