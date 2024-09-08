#include <iostream>

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

// see https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb_image_write.h"

using std::cin, std::cout, std::endl, std::cerr;

const int kNumChannels = 4;

int main(int argc, char* argv[]) {
  // handle CLI arguments
  if (argc != 3) {
    cerr << "invalid arguments" << endl;
    return EXIT_FAILURE;
  }
  char* input = argv[1];
  char* output = argv[2];

  // read image
  int x, y, n;
  unsigned char* data = stbi_load(input, &x, &y, &n, kNumChannels);
  if (data == NULL) {
    cerr << "failed to load image" << endl;
    return EXIT_FAILURE;
  }
  cout << x << endl;
  cout << y << endl;
  cout << n << endl;

  // write image
  int result =
      stbi_write_png(output, x, y, kNumChannels, data, y * kNumChannels);
  if (data == 0) {
    cerr << "failed to write image" << endl;
    return EXIT_FAILURE;
  }

  // TODO exec even if there is an error
  stbi_image_free(data);
}