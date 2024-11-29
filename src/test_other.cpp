#include "../libraries/catch_amalgamated.hpp"

#include "matrix.hpp"
#include "block.hpp"
#include "png.hpp"

namespace p2a = pic2ascii;

TEST_CASE("draw all characters") {
  auto block = p2a::Block(128);
  p2a::PNG png("input/white.png");
  const unsigned int n =
      sizeof(p2a::kAllCharacters) / sizeof(*p2a::kAllCharacters);

  // if this assertion fails, you should prepare larger image
  REQUIRE_NOTHROW(png.ReadNthBlock(n, block));

  for (auto idx = 0u; idx < n; ++idx) {
    png.ReadNthBlock(idx, block);
    REQUIRE_NOTHROW(block.Draw(p2a::kAllCharacters[idx]));
    png.WriteNthBlock(idx, block);
  }

  // manually verify output image
  png.Save("output/test_other_draw_all_characters.png");
}

TEST_CASE("MSSIM does not change") {
  const unsigned int width = 128;
  auto block_slash = p2a::Block(width);
  p2a::PNG png_slash("./input/slash.png");
  png_slash.ReadNthBlock(0, block_slash);

  auto block_pipe = p2a::Block(width);
  p2a::PNG png_pipe("./input/pipe.png");
  png_pipe.ReadNthBlock(0, block_pipe);

  REQUIRE_THAT(block_slash.MSSIM(block_pipe),
               Catch::Matchers::WithinAbs(0.460713, 0.001));
}
