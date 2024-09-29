#include "../libraries/catch_amalgamated.hpp"
#include "pixel.hpp"

TEST_CASE("construct pixel by default constructor") {
  Pixel pixel;
  REQUIRE(pixel.red == 0);
  REQUIRE(pixel.green == 0);
  REQUIRE(pixel.blue == 0);
  REQUIRE(pixel.alpha == 0);
}

TEST_CASE("construct pixel by rgb") {
  Pixel pixel(1, 2, 3);
  REQUIRE(pixel.red == 1);
  REQUIRE(pixel.green == 2);
  REQUIRE(pixel.blue == 3);
  REQUIRE(pixel.alpha == 0);
}

TEST_CASE("construct pixel by rgba") {
  Pixel pixel(1, 2, 3, 4);
  REQUIRE(pixel.red == 1);
  REQUIRE(pixel.green == 2);
  REQUIRE(pixel.blue == 3);
  REQUIRE(pixel.alpha == 4);
}

TEST_CASE("clear pixel") {
  Pixel pixel(1, 1, 1, 1);
  pixel.Clear();
  REQUIRE(pixel.red == 0);
  REQUIRE(pixel.green == 0);
  REQUIRE(pixel.blue == 0);
  REQUIRE(pixel.alpha == 0);
}
