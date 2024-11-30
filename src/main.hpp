namespace pic2ascii {

class Args {
 public:
  std::string input_file_;
  std::string output_file_;
  unsigned int block_width_;
  bool transparent_;
  Args(int argc, char* argv[]);
};

std::tuple<std::string, std::string> split(std::string str, std::string ch);
std::vector<Block> characters(unsigned int block_width);

}  // namespace pic2ascii
