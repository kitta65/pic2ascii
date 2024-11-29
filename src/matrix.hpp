
#ifndef kitta65_pic2ascii_matrix_hpp
#define kitta65_pic2ascii_matrix_hpp

namespace pic2ascii {

class Matrix {
 public:
  unsigned int width;
  unsigned int height;
  std::vector<unsigned int> data;

  Matrix(unsigned int width, unsigned int height);
  unsigned int& operator[](unsigned int index);
  unsigned int& operator[](
      std::tuple<unsigned int, unsigned int> xy);  // NOTE (0, 0) is top-left
  Matrix operator*(Matrix& other);
};

}  // namespace pic2ascii

#endif
