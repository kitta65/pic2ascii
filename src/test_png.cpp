#include "test_png.hpp"

namespace p2a = pic2ascii;

TEST_CASE("constructor") {
  auto png = p2a::PNG("./input/black_32x32.png");
  REQUIRE(png.width_ == 32);
  REQUIRE(png.height_ == 32);
}

TEST_CASE("ReadNthBlock remainder == 0") {
  auto png = p2a::PNG("./input/black_32x32.png");
  auto block = p2a::Block(8);
  for (auto i = 0u; i < 8; ++i) {
    REQUIRE_NOTHROW(png.ReadNthBlock(i, block));
  }
  REQUIRE_THROWS(png.ReadNthBlock(8, block));
}

TEST_CASE("ReadNthBlock remainder != 0") {
  auto png = p2a::PNG("./input/black_32x32.png");
  auto block = p2a::Block(10);
  for (auto i = 0u; i < 8; ++i) {
    REQUIRE_NOTHROW(png.ReadNthBlock(i, block));
  }
  REQUIRE(block.Get({1, 1}) == 0);    // should be black
  REQUIRE(block.Get({2, 2}) == 255);  // should be white
  REQUIRE_THROWS(png.ReadNthBlock(8, block));
}

TEST_CASE("ReadNthBlock out of range") {
  auto png = p2a::PNG("./input/black_32x32.png");
  auto block = p2a::Block(8u);
  REQUIRE_THROWS(png.ReadNthBlock(1000, block));
}
