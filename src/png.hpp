#ifndef kitta65_pic2ascii_png_hpp
#define kitta65_pic2ascii_png_hpp

namespace pic2ascii {

class PNG {
 public:
  unsigned int width;
  unsigned int height;
  unsigned char* data;

  PNG(const char* file);
  virtual ~PNG();
  unsigned int MaxX(const Block& block);
  unsigned int MaxY(const Block& block);
  void ReadNthBlock(unsigned int index, Block& block);
  void ReadNthBlock(unsigned int x, unsigned int y, Block& block);
  void WriteNthBlock(unsigned int index,
                     Block& block,
                     bool transparent = false);
  void WriteNthBlock(unsigned int x,
                     unsigned int y,
                     Block& block,
                     bool transparent = false);
  void Save(const char* file);
};

}  // namespace pic2ascii

#endif
