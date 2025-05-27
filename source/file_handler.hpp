#include <fstream>
#ifndef FILE_HANDLER
#define FILE_HANDLER

class FileHandler {
public:
  // For handling input file
  static std::ifstream create_input_stream(std::string cs_file_path);
  static int get_input_stream_char(std::istream *is);

  // For handling output files
  static std::pair<std::string, std::string>
  get_class_node_output_file_paths(const std::string &class_name);
  static std::pair<std::string, std::string>
  get_class_node_output_file_paths(const std::string &class_name, const std::string &directory_name);
  static std::ofstream open_output_stream(const std::string &file_path);
  static void close_output_stream(std::ofstream &os);
};

#endif