#include "../libraries/catch_amalgamated.hpp"

#include "block.hpp"
#include "png.hpp"

TEST_CASE("draw all characters") {
  auto block = Block(128);
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

TEST_CASE("MSSIM does not change") {
  const unsigned int width = 128;
  auto block_slash = Block(width);
  PNG png_slash("./input/slash.png");
  png_slash.ReadNthBlock(0, block_slash);

  auto block_pipe = Block(width);
  PNG png_pipe("./input/pipe.png");
  png_pipe.ReadNthBlock(0, block_pipe);

  REQUIRE_THAT(block_slash.MSSIM(block_pipe),
               Catch::Matchers::WithinAbs(0.460713, 0.001));
}
