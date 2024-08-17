#include "pixel.hpp"
#include "utils.hpp"

// see https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
#include "./libraries/catch_amalgamated.hpp"

TEST_CASE("sum of two values", "[sum]") { REQUIRE(my_sum(1, 2) == 3); }

TEST_CASE("construct pixel") {
  Pixel pixel = {0, 0, 0};
  REQUIRE(pixel.red == 0);
}