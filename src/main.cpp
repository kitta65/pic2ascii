#include <iostream>
#include <algorithm>
#include <vector>

#include "pixel.hpp"
#include "block.hpp"
#include "png.hpp"

const int kBlockWidth = 4;
const int kBlockHeight = kBlockWidth * 2;

int main(int argc, char* argv[]) {
  // handle CLI arguments
  if (argc != 3) {
    std::cerr << "invalid arguments" << std::endl;
    return EXIT_FAILURE;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];

  auto pixels = std::vector<Pixel>(kBlockWidth * kBlockHeight);
  auto block = Block(kBlockWidth, kBlockHeight, &pixels);

  // read and edit image
  PNG png(input_file);
  for (auto idx = 0u; png.ReadNthBlock(idx, block); ++idx) {
    block.Clear();
    png.WriteNthBlock(idx, block);
  }
  png.Save(output_file);
}