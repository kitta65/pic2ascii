#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "block.hpp"

TEST_CASE("construct block") {
  Block block(1, 1);
  REQUIRE(block.width == 1);
  REQUIRE(block.height == 1);
}

TEST_CASE("operator[]") {
  Block block(0, 0);
  REQUIRE_THROWS(block[{1, 1}]);
}

TEST_CASE("draw characters") {
  const auto width = 16;
  const auto height = width * 2;
  Block block(width, height);

  for (auto c : kAllCharacters) {
    REQUIRE_NOTHROW(block.Draw(c));
  }
}

TEST_CASE("MSSIM fail") {
  const unsigned int width_a = 16;
  const unsigned int height_a = width_a * 2;
  Block block_a(width_a, height_a);

  const unsigned int width_b = 8;
  const unsigned int height_b = width_b * 2;
  Block block_b(width_b, height_b);

  REQUIRE_THROWS(block_a.MSSIM(block_b));
}

TEST_CASE("MSSIM 1.0") {
  const unsigned int width = 16;
  const unsigned int height = width * 2;
  Block block_x(width, height);
  Block block_y(width, height);
  for (auto w = 0u; w < width; ++w) {
    for (auto h = 0u; h < height; ++h) {
      block_x[{w, h}] = 0;
      block_y[{w, h}] = 0;
    }
  }
  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(1.0, 0.001));
}

TEST_CASE("MSSIM 0.0") {
  const unsigned int width = 16;
  const unsigned int height = width * 2;
  Block block_x(width, height);
  Block block_y(width, height);
  for (auto w = 0u; w < width; ++w) {
    for (auto h = 0u; h < height; ++h) {
      block_x[{w, h}] = 0;
      block_y[{w, h}] = 255;
    }
  }
  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(0.0, 0.001));
}
