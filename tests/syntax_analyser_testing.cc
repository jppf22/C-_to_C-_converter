#include "../source/custom_exceptions.hpp"
#include "../source/file_handler.hpp"
#include "../source/lexer.hpp"
#include "../source/parser.hpp"
#include <sstream>
#include <gtest/gtest.h>

std::string runSyntaxAnalyser(const std::string& filename) {
    std::ifstream input_file_stream(filename);
    if (!input_file_stream) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    Lexer lexer(&input_file_stream);
    Parser parser(lexer);

    // This will throw if there's an issue
    auto class_nodes = parser.parseProgram();

    return "OK"; // Return something to indicate success
}

// --- TESTS ---

TEST(SyntaxAnalyserTests, UnknownToken) {
    std::string code = 
    R"(
    class A {
        public A() { }
        $illegal = 42;
    }
    )";
    
    std::istringstream stream(code);
    Lexer lexer(&stream);
    Parser parser(lexer);

    try {
        parser.parseProgram();
        FAIL() << "Expected Parser_Exception";
    } catch (const Parser_Exception& e) {
        std::ostringstream oss;

        oss << "Parser_Exception: ";
        oss << "Unrecognized Symbol \'$\'.";
        oss << " Line: 4 " << "Column: 9";  

        std::string expected_msg = oss.str();
        EXPECT_STREQ(e.what(), expected_msg.c_str());
    }
}

TEST(SyntaxAnalyserTests, MissingClosingBracketThrows) {
    std::string code = R"(
        class A {
            public int x;
        )";
    std::istringstream stream(code);
    Lexer lexer(&stream);
    Parser parser(lexer);

    try {
        parser.parseProgram();
        FAIL() << "Expected Parser_Exception for missing '}'";
    } catch (const Parser_Exception& e) {

        std::ostringstream oss;
        oss << "Parser_Exception: Expected token \'}\'.";
        oss << " Line: 3 " << "Column: 25";

        EXPECT_STREQ(e.what(), oss.str().c_str());
    }
}

TEST(SyntaxAnalyserTests, MissingClassNameThrows) {
    std::string code = R"(
        class {
            public void A() { }
        }
    )";
    std::istringstream stream(code);
    Lexer lexer(&stream);
    Parser parser(lexer);

    try {
        parser.parseProgram();
        FAIL() << "Expected Parser_Exception for missing class name";
    } catch (const Parser_Exception& e) {

        std::ostringstream oss;
        oss << "Parser_Exception: Expected Identifier at char \'\n\'.";
        oss << " Line: 2 " << "Column: 15";

        EXPECT_STREQ(e.what(), oss.str().c_str());
    }
}

TEST(SyntaxAnalyserTests, MemberOutsideClassThrows) {
    std::string code = R"(
        int x = 10;
        class A { }
    )";
    std::istringstream stream(code);
    Lexer lexer(&stream);
    Parser parser(lexer);

    try {
        parser.parseProgram();
        FAIL() << "Expected Parser_Exception for member outside class";
    } catch (const Parser_Exception& e) {
        
        std::ostringstream oss;
        oss << "Parser_Exception: Expected token \'class\'.";
        oss << " Line: 2 " << "Column: 9";

        EXPECT_STREQ(e.what(), oss.str().c_str());
    }
}