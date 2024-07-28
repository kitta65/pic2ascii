#include "utils.hpp"

// see https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
#include "./libraries/catch_amalgamated.hpp"

TEST_CASE("sum of two values", "[sum]") {
  REQUIRE(my_sum(1, 2) == 3);
}