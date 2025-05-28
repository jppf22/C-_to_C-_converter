#include "file_handler.hpp"
#include "custom_exceptions.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// For handling input files
std::ifstream FileHandler::create_input_stream(std::string cs_file_path) {
  std::ifstream is{cs_file_path};

  if (is.fail()) {
    throw IO_Exception("Failed to open the input file stream");
  }

  return is;
}

int FileHandler::get_input_stream_char(std::istream *is) {
  int c = is->get(); 
  if (c == EOF) {
    return EOF;
  } else if (is->fail()) {
    throw IO_Exception(
        "Failed while reading a character from the input file stream");
  } else {
    return c; 
  }
}

// For handling output files

std::pair<std::string, std::string>
FileHandler::get_class_node_output_file_paths(const std::string &class_name) {
  return get_class_node_output_file_paths(class_name, ".");
}

std::pair<std::string, std::string>
FileHandler::get_class_node_output_file_paths(
    const std::string &class_name, const std::string &directory_name) {
  try {
    fs::path dir_path(directory_name);
    if (!fs::exists(dir_path)) {
      fs::create_directories(dir_path);
    }

    fs::path header_path = dir_path / (class_name + ".hpp");
    fs::path source_path = dir_path / (class_name + ".cpp");

    return {header_path.string(), source_path.string()};
  } catch (const fs::filesystem_error &e) {
    throw IO_Exception(("Filesystem error: " + std::string(e.what())).c_str());
  }
}

std::ofstream FileHandler::open_output_stream(const std::string &file_path) {
  std::ofstream os{file_path};
  if (os.fail()) {
    throw IO_Exception("Failed to open the output file stream");
  }
  return os;
}

void FileHandler::close_output_stream(std::ofstream &os) {
  if (os.is_open()) {
    os.close();
  }
}