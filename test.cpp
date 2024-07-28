// see https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
#include "./libraries/catch_amalgamated.hpp"

unsigned int MySum(unsigned int lhs, unsigned int rhs) { return lhs + rhs; }

TEST_CASE("sum of two values", "[sum]") {
  REQUIRE(MySum(1, 2) == 3);
}