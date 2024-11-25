#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "block.hpp"

namespace p2a = pic2ascii;

TEST_CASE("construct block") {
  p2a::Block block(12);
  REQUIRE(block.width == 12);
  REQUIRE(block.height == 24);
  REQUIRE(block.filter_size == 3);
}

TEST_CASE("operator[]") {
  p2a::Block block(1);
  REQUIRE_THROWS(block[{1, 1}]);
}

TEST_CASE("draw characters") {
  const auto width = 16;
  p2a::Block block(width);

  for (auto c : p2a::kAllCharacters) {
    REQUIRE_NOTHROW(block.Draw(c));
  }
}

TEST_CASE("filter") {
  p2a::Block original(12);  // filter_size should be 3
  original.Clear();
  original[{1, 1}] = 255 - 18;

  auto filtered = original.Filter();
  unsigned int expected = filtered[{1, 1}];
  REQUIRE(expected == 255 - 2);
}

TEST_CASE("MSSIM fail") {
  p2a::Block block_a(16);
  p2a::Block block_b(8);
  REQUIRE_THROWS(block_a.MSSIM(block_b));
}

TEST_CASE("MSSIM 1.0") {
  const unsigned int width = 16;
  p2a::Block block_x(width);
  p2a::Block block_y(width);
  for (auto w = 0u; w < block_x.width; ++w) {
    for (auto h = 0u; h < block_x.height; ++h) {
      block_x[{w, h}] = 0;
      block_y[{w, h}] = 0;
    }
  }
  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(1.0, 0.001));
}

TEST_CASE("MSSIM 0.0") {
  const unsigned int width = 16;
  p2a::Block block_x(width);
  p2a::Block block_y(width);
  for (auto w = 0u; w < block_x.width; ++w) {
    for (auto h = 0u; h < block_x.height; ++h) {
      block_x[{w, h}] = 0;
      block_y[{w, h}] = 255;
    }
  }
  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(0.0, 0.001));
}
