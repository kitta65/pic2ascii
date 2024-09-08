#ifndef kitta65_pic2ascii_pixel_hpp
#define kitta65_pic2ascii_pixel_hpp

class Pixel {
 private:
  unsigned char red_;
  unsigned char green_;
  unsigned char blue_;
  unsigned char alpha_;

 public:
  // read only members
  const unsigned char& red = red_;
  const unsigned char& green = green_;
  const unsigned char& blue = blue_;
  const unsigned char& alpha = alpha_;

  // constructors
  Pixel();
  Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);
};

#endif