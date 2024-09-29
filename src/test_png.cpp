#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "pixel.hpp"
#include "block.hpp"
#include "png.hpp"

TEST_CASE("constructor") {
  auto png = PNG("./input/black.png");
  REQUIRE(png.width == 32);
  REQUIRE(png.height == 32);
}

TEST_CASE("ReadNthBlock remainder == 0") {
  const auto width = 8u;
  const auto height = 8u;

  auto png = PNG("./input/black.png");
  auto pixels = std::vector<Pixel>(width * height);
  auto block = Block(width, height, &pixels);
  auto idx = 0u;
  while (png.ReadNthBlock(idx, block)) {
    ++idx;
  }

  REQUIRE(idx == 16);
}

TEST_CASE("ReadNthBlock remainder != 0") {
  const auto width = 10u;
  const auto height = 10u;

  auto png = PNG("./input/black.png");
  auto pixels = std::vector<Pixel>(width * height);
  auto block = Block(width, height, &pixels);
  auto idx = 0u;
  while (png.ReadNthBlock(idx, block)) {
    ++idx;
  }

  REQUIRE(idx == 16);
  REQUIRE(block[{1, 1}].alpha == 255);  // should be black
  REQUIRE(block[{2, 2}].alpha == 0);    // should be transparent
}
