#include <iostream>
#include <algorithm>

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

  // read and edit image
  PNG png(input_file);
  for (auto i = 0u; true; ++i) {
    auto block = png.GetNthBlock(i, kBlockWidth, kBlockHeight);
    if (block.pixels == NULL) {
      break;
    }
    block.Clear();
    png.SetNthBlock(i, block);
  }
  png.Save(output_file);
}