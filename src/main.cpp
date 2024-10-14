#include <iostream>
#include <vector>

#include "block.hpp"
#include "png.hpp"

const int kDefaultBlockWidth = 4;

int main(int argc, char* argv[]) {
  auto blockWidth = kDefaultBlockWidth;

  // handle CLI arguments
  if (argc < 3 || 5 <= argc) {
    std::cerr << "invalid arguments" << std::endl;
    return EXIT_FAILURE;
  } else if (argc == 4) {
    blockWidth = atoi(argv[3]);
  }
  const auto input_file = argv[1];
  const auto output_file = argv[2];

  // read and edit image
  auto pixels = std::vector<unsigned char>(blockWidth * (blockWidth * 2));
  auto block = Block(blockWidth, blockWidth * 2, &pixels);
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
