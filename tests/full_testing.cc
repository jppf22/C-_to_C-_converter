#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "../source/code_generator.hpp"
#include "../source/custom_exceptions.hpp"
#include "../source/file_handler.hpp"
#include "../source/lexer.hpp"
#include "../source/parser.hpp"
#include "../source/validator.hpp"

namespace fs = std::filesystem;

// Helper function: read whole file to string
std::string read_file(const fs::path &path) {
  std::ifstream file(path);
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

// Runs full pipeline on one input and compares generated output to expected
// output
void run_test_case(const std::string &test_num) {
  fs::path input_path =
      fs::path(TEST_BINARY_DIR) / "tests/inputs" / (test_num + ".cs");
  fs::path expected_dir =
      fs::path(TEST_BINARY_DIR) / "tests/outputs" / test_num;

  ASSERT_TRUE(fs::exists(input_path)) << "Input file missing: " << input_path;
  ASSERT_TRUE(fs::exists(expected_dir))
      << "Expected output folder missing: " << expected_dir;

  // Open input stream as in main
  std::ifstream input_stream =
      FileHandler::create_input_stream(input_path.string());
  ASSERT_TRUE(input_stream.is_open()) << "Failed to open input: " << input_path;

  try {
    Lexer lexer(&input_stream);
    Parser parser(lexer);
    std::vector<ClassNode> class_nodes = parser.parseProgram();

    Validator::ensure_valid_structure(class_nodes);

    for (ClassNode &class_node : class_nodes) {
      // Instead of writing to files, write to stringstreams
      std::ostringstream gen_header_ss;
      CodeGenerator::generate_header(class_node, gen_header_ss);
      std::string generated_header = gen_header_ss.str();

      std::ostringstream gen_source_ss;
      CodeGenerator::generate_source(class_node, gen_source_ss);
      std::string generated_source = gen_source_ss.str();

      // Load expected outputs
      fs::path expected_header_path = expected_dir / (class_node.name + ".hpp");
      fs::path expected_source_path = expected_dir / (class_node.name + ".cpp");

      ASSERT_TRUE(fs::exists(expected_header_path))
          << "Expected header missing: " << expected_header_path;
      ASSERT_TRUE(fs::exists(expected_source_path))
          << "Expected source missing: " << expected_source_path;

      std::string expected_header = read_file(expected_header_path);
      std::string expected_source = read_file(expected_source_path);

      // Compare with Google Test assertions
      EXPECT_EQ(generated_header, expected_header)
          << "Header mismatch in test " << test_num << " for class "
          << class_node.name;
      EXPECT_EQ(generated_source, expected_source)
          << "Source mismatch in test " << test_num << " for class "
          << class_node.name;
    }

  } catch (const IO_Exception &e) {
    FAIL() << "IO_Exception: " << e.what();
  } catch (const Parser_Exception &e) {
    FAIL() << "Parser_Exception: " << e.what();
  } catch (const Validator_Exception &e) {
    FAIL() << "Validator_Exception: " << e.what();
  } catch (const Code_Generation_Exception &e) {
    FAIL() << "Code_Generation_Exception: " << e.what();
  } catch (const std::exception &e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

// Parameterized Google Test class
class FullPipelineTest : public ::testing::TestWithParam<std::string> {};

TEST_P(FullPipelineTest, OutputMatchesExpected) { run_test_case(GetParam()); }

// Instantiate for tests 1 through 25
INSTANTIATE_TEST_SUITE_P(RunAllTests, FullPipelineTest,
                         ::testing::Values("1", "2", "3", "4", "5", "6", "7",
                                           "8", "9", "10", "11", "12", "13",
                                           "14", "15", "16", "17", "18", "19",
                                           "20", "21", "22", "23", "24", "25"));
