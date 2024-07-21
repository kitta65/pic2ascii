#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "./libraries/stb_image.h"

using std::cout, std::endl;

int main() {
  int x, y, n;
  unsigned char *data = stbi_load("./pictures/black.png", &x, &y, &n, 4);
  cout << x << endl;
  cout << y << endl;
  cout << n << endl;
  stbi_image_free(data);
}