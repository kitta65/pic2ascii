#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "block.hpp"

TEST_CASE("construct block") {
  Block block(1);
  REQUIRE(block.width == 1);
  REQUIRE(block.height == 2);
}

TEST_CASE("operator[]") {
  Block block(1);
  REQUIRE_THROWS(block[{1, 1}]);
}

TEST_CASE("draw characters") {
  const auto width = 16;
  Block block(width);

  for (auto c : kAllCharacters) {
    REQUIRE_NOTHROW(block.Draw(c));
  }
}

TEST_CASE("MSSIM fail") {
  Block block_a(16);
  Block block_b(8);
  REQUIRE_THROWS(block_a.MSSIM(block_b));
}

TEST_CASE("MSSIM 1.0") {
  const unsigned int width = 16;
  Block block_x(width);
  Block block_y(width);
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
  Block block_x(width);
  Block block_y(width);
  for (auto w = 0u; w < block_x.width; ++w) {
    for (auto h = 0u; h < block_x.height; ++h) {
      block_x[{w, h}] = 0;
      block_y[{w, h}] = 255;
    }
  }
  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(0.0, 0.001));
}
