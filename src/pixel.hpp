#ifndef kitta65_pic2ascii_pixel_hpp
#define kitta65_pic2ascii_pixel_hpp

class Pixel {
 public:
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;

  // constructors
  Pixel();
  Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);

  void Clear();
};

#endif