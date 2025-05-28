
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "code_generator.hpp"
#include "custom_exceptions.hpp"
#include "file_handler.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "validator.hpp"

#define OUTPUT_DIRECTORY "output"

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
    std::ifstream input_file_stream =
        FileHandler::create_input_stream(cs_file_path);

    Lexer lexer(&input_file_stream);
    Parser parser(lexer);
    std::vector<ClassNode> class_nodes = parser.parseProgram();

    std::cout << "---------------- CLASS NODES ------------------\n\n";
    for (long unsigned int i = 0; i < class_nodes.size(); i++) {
      std::cout << i + 1 << " ------------------\n"
                << class_nodes[i] << "\n------------------\n";
    }
    std::cout << "Full AST Tree constructed!\n";

    std::cout << "---------------- VALIDATION ------------------\n\n";
    Validator::ensure_valid_structure(class_nodes);
    std::cout << "All classes valid!\n";

    std::cout << "---------------- CODE GENERATION ------------------\n\n";

    for (ClassNode &class_node : class_nodes) {
      auto [header_path, source_path] =
          FileHandler::get_class_node_output_file_paths(class_node.name,
                                                        "output");

      std::ofstream header_stream =
          FileHandler::open_output_stream(header_path);
      CodeGenerator::generate_header(class_node, header_stream);
      FileHandler::close_output_stream(header_stream);

      std::ofstream source_stream =
          FileHandler::open_output_stream(source_path);
      CodeGenerator::generate_source(class_node, source_stream);
      FileHandler::close_output_stream(source_stream);

      std::cout << "Generated files: \n-" << header_path << "\n-" << source_path
                << "\n\n";
    }

  } catch (const IO_Exception &e) {
    std::cerr << e.what() << '\n';
  } catch (const Parser_Exception &e) {
    std::cerr << e.what() << '\n';
  } catch (const Validator_Exception &e) {
    std::cerr << e.what() << '\n';
  } catch (const Code_Generation_Exception &e) {
    std::cerr << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "Caught unexpected behavior: " << e.what() << '\n';
  }

  return 0;
}