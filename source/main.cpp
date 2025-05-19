
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "custom_exceptions.hpp"
#include "file_handler.hpp"
#include "tokenizer.hpp"

int main(int argc, char *argv[]) {

  if (argc == 1 || argc > 2) {
    std::cerr << "To run the program you need to provide the .cs file path "
                 "through the command line. Ex.: \"./main example.cs\". \n";
    return EXIT_FAILURE;
  }

  std::string cs_file_path{argv[1]};

  if (cs_file_path.length() <= 3 ||
      cs_file_path.substr(cs_file_path.length() - 3, 3) != ".cs") {
    std::cerr << "Must provide a .cs file path. This program only works for C# "
                 "files. \n";
    return EXIT_FAILURE;
  }

  // After all checks on program call, may proceed with program logic

  try {
    std::ifstream input_file_stream = create_input_stream(cs_file_path);
    std::vector<Token> symbol_table =
        construct_symbol_table(&input_file_stream);
  } catch (const IO_Exception &e) {
    std::cerr << e.what() << '\n';
  } catch (const Tokenizer_Exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}