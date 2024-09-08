#include <iostream>

#include "utils.hpp"

// see https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

using std::cin, std::cout, std::endl, std::cerr;

int main(int argc, char* argv[]) {
  // handle CLI arguments
  if (argc != 2) {
    cerr << "invalid arguments" << endl;
    return EXIT_FAILURE;
  }
  char* filename = argv[1];

  // load image data
  int x, y, n;
  unsigned char* data = stbi_load(filename, &x, &y, &n, 4);
  if (data == NULL) {
    cerr << "failed to load image" << endl;
    return EXIT_FAILURE;
  }
  cout << x << endl;
  cout << y << endl;
  cout << n << endl;
  stbi_image_free(data);

  // try to use functions in utils.cpp
  cout << "my_sum(1, 2) = " << my_sum(1, 2) << endl;
}