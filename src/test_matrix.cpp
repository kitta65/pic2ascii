#include "test_matrix.hpp"

namespace p2a = pic2ascii;

TEST_CASE("constructor") {
  auto matrix = p2a::Matrix(1, 1);
  REQUIRE_NOTHROW(matrix[{0, 0}]);
  REQUIRE_THROWS(matrix[{1, 1}]);
}

TEST_CASE("operator* success") {
  auto left = p2a::Matrix(1, 1);
  left[{0, 0}] = 2;
  auto right = p2a::Matrix(1, 1);
  right[{0, 0}] = 3;

  auto result = left * right;
  REQUIRE(result.width_ == 1);
  REQUIRE(result.height_ == 1);
  REQUIRE(result[{0, 0}] == 6);
}

TEST_CASE("operator* fail") {
  auto left = p2a::Matrix(1, 1);
  auto right = p2a::Matrix(1, 2);

  REQUIRE_THROWS(left * right);
}
