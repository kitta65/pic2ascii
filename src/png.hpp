#ifndef kitta65_pic2ascii_png_hpp
#define kitta65_pic2ascii_png_hpp

class PNG {
 public:
  unsigned int width;
  unsigned int height;
  unsigned char* data;

  PNG(char* file);
  ~PNG();
  void Save(char* file);
  Block GetNthBlock(unsigned int index, unsigned int width, unsigned int height);
  void SetNthBlock(unsigned int index, Block& block);
};

#endif