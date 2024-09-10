#include <iostream>
#include <algorithm>

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

#include "pixel.hpp"
#include "block.hpp"

using std::cin, std::cout, std::endl, std::cerr;

const int kNumChannels = 4;
const int kBlockWidth = 4;
const int kBlockHeight = kBlockWidth * 2;

int main(int argc, char* argv[]) {
  // handle CLI arguments
  if (argc != 3) {
    cerr << "invalid arguments" << endl;
    return EXIT_FAILURE;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];

  // read image
  int width, height, n;
  unsigned char* input_data =
      stbi_load(input_file, &width, &height, &n, kNumChannels);
  if (input_data == NULL) {
    cerr << "failed to load image" << endl;
    return EXIT_FAILURE;
  }

  // edit image
  unsigned char output_data[width * height * kNumChannels];
  std::copy(input_data, input_data + width * height * kNumChannels,
            output_data);
  for (auto block_y = 0u; block_y < height / kBlockHeight; ++block_y) {
    for (auto block_x = 0u; block_x < width / kBlockWidth; ++block_x) {
      const auto base_x = block_x * kBlockWidth;
      const auto base_y = block_y * kBlockHeight;
      const auto base_idx = (base_x + base_y * width) * kNumChannels;

      Pixel pixels[kBlockWidth * kBlockHeight];
      for (auto pixel_y = 0u; pixel_y < kBlockHeight; ++pixel_y) {
        auto offset_idx = pixel_y * width * kNumChannels;
        for (auto pixel_x = 0u; pixel_x < kBlockWidth; ++pixel_x) {
          pixels[pixel_x + pixel_y * kBlockWidth] =
              Pixel(input_data[base_idx + offset_idx + 0],
                    input_data[base_idx + offset_idx + 1],
                    input_data[base_idx + offset_idx + 2],
                    input_data[base_idx + offset_idx + 3]);
          offset_idx += kNumChannels;
        }
      }

      Block block(kBlockWidth, kBlockHeight, pixels);
      block.Clear();

      for (auto pixel_y = 0u; pixel_y < kBlockHeight; ++pixel_y) {
        auto offset_idx = pixel_y * width * kNumChannels;
        for (auto pixel_x = 0u; pixel_x < kBlockWidth; ++pixel_x) {
          auto pixel = block[{pixel_x, pixel_y}];
          auto idx = 0u;
          output_data[base_idx + offset_idx + 0] = pixel.red;
          output_data[base_idx + offset_idx + 1] = pixel.green;
          output_data[base_idx + offset_idx + 2] = pixel.blue;
          output_data[base_idx + offset_idx + 3] = pixel.alpha;
        }
      }
    }
  }

  // write image
  int result = stbi_write_png(output_file, width, height, kNumChannels,
                              output_data, width * kNumChannels);
  if (result == 0) {
    cerr << "failed to write image" << endl;
    stbi_image_free(input_data);
    return EXIT_FAILURE;
  }

  stbi_image_free(input_data);
}