namespace pic2ascii {

class Args {
 public:
  std::string input_file;
  std::string output_file;
  unsigned int block_width;
  bool transparent;
  Args(int argc, char* argv[]);
};

std::tuple<std::string, std::string> split(std::string str, std::string ch);
std::vector<Block> characters(unsigned int block_width);
const char* print(Character);

}  // namespace pic2ascii
