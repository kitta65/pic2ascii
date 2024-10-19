#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "block.hpp"

TEST_CASE("construct block") {
  auto pixels = std::vector<unsigned char>(1);
  pixels[0] = 255;
  Block block(1, 1, &pixels);
  REQUIRE(block[{0, 0}] == 255);
}

TEST_CASE("operator[]") {
  auto pixels = std::vector<unsigned char>(0);
  Block block(0, 0, &pixels);
  REQUIRE_THROWS(block[{1, 1}]);
}

TEST_CASE("draw characters") {
  const auto width = 16;
  const auto height = width * 2;
  auto pixels = std::vector<unsigned char>(width * height);
  Block block(width, height, &pixels);

  for (auto c : kAllCharacters) {
    REQUIRE_NOTHROW(block.Draw(c));
  }
}

TEST_CASE("MSSIM fail") {
  const unsigned int width_a = 16;
  const unsigned int height_a = width_a * 2;
  auto pixels_a = std::vector<unsigned char>(width_a * height_a);
  Block block_a(width_a, height_a, &pixels_a);

  const unsigned int width_b = 8;
  const unsigned int height_b = width_b * 2;
  auto pixels_b = std::vector<unsigned char>(width_b * height_b);
  Block block_b(width_b, height_b, &pixels_b);

  REQUIRE_THROWS(block_a.MSSIM(block_b));
}

TEST_CASE("MSSIM 1.0") {
  const unsigned int width = 16;
  const unsigned int height = width * 2;
  auto pixels_x = std::vector<unsigned char>(width * height);
  auto pixels_y = std::vector<unsigned char>(width * height);
  Block block_x(width, height, &pixels_x);
  Block block_y(width, height, &pixels_y);
  for (auto idx = 0; idx < width * height; ++idx) {
    pixels_x[idx] = 0;
    pixels_y[idx] = 0;
  }

  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(1.0, 0.001));
}

TEST_CASE("MSSIM 0.0") {
  const unsigned int width = 16;
  const unsigned int height = width * 2;
  auto pixels_x = std::vector<unsigned char>(width * height);
  auto pixels_y = std::vector<unsigned char>(width * height);
  Block block_x(width, height, &pixels_x);
  Block block_y(width, height, &pixels_y);
  for (auto idx = 0; idx < width * height; ++idx) {
    pixels_x[idx] = 0;
    pixels_y[idx] = 255;
  }

  auto mssim = block_x.MSSIM(block_y);
  REQUIRE_THAT(mssim, Catch::Matchers::WithinAbs(0.0, 0.001));
}
