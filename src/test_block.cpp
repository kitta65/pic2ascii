#include <vector>
#include "pixel.hpp"
#include "block.hpp"
#include "../libraries/catch_amalgamated.hpp"

TEST_CASE("construct block") {
  auto pixels = std::vector<Pixel>(1);
  pixels[0] = Pixel(1, 2, 3, 4);
  Block block(1, 1, &pixels);
  REQUIRE(block[{0, 0}].red == 1);
  REQUIRE(block[{0, 0}].green == 2);
  REQUIRE(block[{0, 0}].blue == 3);
  REQUIRE(block[{0, 0}].alpha == 4);
}

TEST_CASE("operator[]") {
  auto pixels = std::vector<Pixel>(0);
  Block block(0, 0, &pixels);
  REQUIRE_THROWS(block[{1, 1}]);
}