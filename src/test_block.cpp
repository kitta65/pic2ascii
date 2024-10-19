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

TEST_CASE("ssim fail") {
  const unsigned int width_a = 16;
  const unsigned int height_a = width_a * 2;
  auto pixels_a = std::vector<unsigned char>(width_a * height_a);
  Block block_a(width_a, height_a, &pixels_a);

  const unsigned int width_b = 8;
  const unsigned int height_b = width_b * 2;
  auto pixels_b = std::vector<unsigned char>(width_b * height_b);
  Block block_b(width_b, height_b, &pixels_b);

  REQUIRE_THROWS(block_a.SSIM(block_b));
}
