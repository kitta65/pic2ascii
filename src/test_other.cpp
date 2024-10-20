#include "../libraries/catch_amalgamated.hpp"

#include "block.hpp"
#include "png.hpp"

TEST_CASE("draw all characters") {
  auto block = Block(123);
  PNG png("input/white.png");
  const unsigned int n = sizeof(kAllCharacters) / sizeof(*kAllCharacters);

  // if this assertion failse, you should prepare larger image
  REQUIRE(png.ReadNthBlock(n, block));

  for (auto idx = 0; idx < n; ++idx) {
    png.ReadNthBlock(idx, block);
    REQUIRE_NOTHROW(block.Draw(kAllCharacters[idx]));
    png.WriteNthBlock(idx, block);
  }

  // manually verify output image
  png.Save("output/test_other_draw_all_characters.png");
}
