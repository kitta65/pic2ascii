#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "block.hpp"
#include "png.hpp"

TEST_CASE("constructor") {
  auto png = PNG("./input/black.png");
  REQUIRE(png.width == 32);
  REQUIRE(png.height == 32);
}

TEST_CASE("ReadNthBlock remainder == 0") {
  auto png = PNG("./input/black.png");
  auto block = Block(8);
  auto idx = 0u;
  while (png.ReadNthBlock(idx, block)) {
    ++idx;
  }

  REQUIRE(idx == 8);
}

TEST_CASE("ReadNthBlock remainder != 0") {
  auto png = PNG("./input/black.png");
  auto block = Block(10);
  auto idx = 0u;
  while (png.ReadNthBlock(idx, block)) {
    ++idx;
  }

  REQUIRE(idx == 8);
  REQUIRE(block[{1, 1}] == 0);    // should be black
  REQUIRE(block[{2, 2}] == 255);  // should be white
}

TEST_CASE("ReadNthBlock out of range") {
  auto png = PNG("./input/black.png");
  auto block = Block(8u);
  REQUIRE_FALSE(png.ReadNthBlock(1000, block));
}
