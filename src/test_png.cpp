#include <vector>
#include "../libraries/catch_amalgamated.hpp"
#include "pixel.hpp"
#include "block.hpp"
#include "png.hpp"

TEST_CASE("construct png") {
  auto png = PNG("./input/black.png");
  REQUIRE(png.width == 32);
  REQUIRE(png.height == 32);
}