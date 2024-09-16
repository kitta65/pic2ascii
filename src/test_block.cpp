#include "pixel.hpp"
#include "block.hpp"
#include "../libraries/catch_amalgamated.hpp"

TEST_CASE("construct block") {
  Pixel pixels[] = {
      Pixel(1, 2, 3, 4),
  };
  Block block(1, 1, pixels);
  REQUIRE(block[{0, 0}].red == 1);
  REQUIRE(block[{0, 0}].green == 2);
  REQUIRE(block[{0, 0}].blue == 3);
  REQUIRE(block[{0, 0}].alpha == 4);
}