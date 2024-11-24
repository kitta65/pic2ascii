#ifndef kitta65_pic2ascii_png_hpp
#define kitta65_pic2ascii_png_hpp

class PNG {
 public:
  unsigned int width;
  unsigned int height;
  unsigned char* data;

  PNG(const char* file);
  ~PNG();
  void Save(const char* file);
  bool ReadNthBlock(unsigned int index, Block& block);
  bool ReadNthBlock(unsigned int x, unsigned int y, Block& block);
  void WriteNthBlock(unsigned int index, Block& block);
  void WriteNthBlock(unsigned int x, unsigned int y, Block& block);
};

#endif
