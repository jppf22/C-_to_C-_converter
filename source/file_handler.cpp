#include "file_handler.hpp"
#include "custom_exceptions.hpp"
#include <fstream>
#include <iostream>

std::ifstream FileHandler::create_input_stream(std::string cs_file_path) {
  std::ifstream is{cs_file_path};

  if (is.fail()) {
    throw IO_Exception("Failed to open the input file stream");
  }

  return is;
}

int FileHandler::get_input_stream_char(std::istream *is) {
  int c = is->get(); // returns int, either 0-255 or EOF (-1)
  if (c == EOF) {
    // EOF reached
    return EOF;
  } else if (is->fail()) {
    throw IO_Exception(
        "Failed while reading a character from the input file stream");
  } else {
    return c; // valid char (promoted to int)
  }
}
