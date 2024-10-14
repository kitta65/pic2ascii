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

  // TODO draw all characters to check manually
  REQUIRE_NOTHROW(block.Draw(PIPE));
}
