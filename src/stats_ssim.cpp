#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "xy.hpp"
#include "matrix.hpp"
#include "block.hpp"

namespace p2a = pic2ascii;

int main() {
  std::fstream file;
  file.open("./jupyter/mssim.tsv", std::ios_base::out);
  if (!file.is_open()) {
    std::cout << "failed to open file" << std::endl;
    return EXIT_FAILURE;
  }

  p2a::Block block1(16);
  p2a::Block block2(16);

  file << "character1" << "\t" << "character2" << "\t" << "mssim" << std::endl;
  for (auto c1 : p2a::kAllCharacters) {
    block1.Draw(c1);
    for (auto c2 : p2a::kAllCharacters) {
      if (c1 > c2) {
        continue;
      }

      block2.Draw(c2);
      auto mssim = block1.MSSIM(block2);
      file << print(c1) << "\t" << print(c2) << "\t" << mssim << std::endl;
    }
  }

  file.close();
}
