#include "source/tokenizer.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <limits.h>
#include <unistd.h>

// CHANGE TO USE ACTUAL TEST FILES EXAMPLES

/*
NormalTable1 - Tests
NormalTable2 - Tests
COMPLETE
*/

TEST(TokenTableTests, NormalTable1) {
  try {

    std::string file_contents = R"(class Derived : Base
{
public Derived()
{

}
public int IntField {get;}
public AnotherClass ClassField {get; set;}
public bool do_work(string param1, double param2, YetAnotherClass param3)
{

}
public override bool Equals(Object other)
{

}
})";

    std::istringstream is(file_contents);

    std::vector<Token> symbol_table = Tokenizer::construct_symbol_table(&is);

    std::vector<Token> expect_symbol_table = {
        {TokenType::Keyword, "class", 1, 1},
        {TokenType::Identifier, "Derived", 1, 7},
        {TokenType::Symbol, ":", 1, 15},
        {TokenType::Identifier, "Base", 1, 17},
        {TokenType::Symbol, "{", 2, 1},
        {TokenType::Keyword, "public", 3, 1},
        {TokenType::Identifier, "Derived", 3, 8},
        {TokenType::Symbol, "(", 3, 15},
        {TokenType::Symbol, ")", 3, 16},
        {TokenType::Symbol, "{", 4, 1},
        {TokenType::Symbol, "}", 5, 1},
        {TokenType::Keyword, "public", 6, 1},
        {TokenType::Keyword, "int", 6, 8},
        {TokenType::Identifier, "IntField", 6, 12},
        {TokenType::Symbol, "{", 6, 21},
        {TokenType::Keyword, "get", 6, 22},
        {TokenType::Symbol, ";", 6, 25},
        {TokenType::Symbol, "}", 6, 26},
        {TokenType::Keyword, "public", 7, 1},
        {TokenType::Identifier, "AnotherClass", 7, 8},
        {TokenType::Identifier, "ClassField", 7, 21},
        {TokenType::Symbol, "{", 7, 32},
        {TokenType::Keyword, "get", 7, 33},
        {TokenType::Symbol, ";", 7, 36},
        {TokenType::Keyword, "set", 7, 38},
        {TokenType::Symbol, ";", 7, 41},
        {TokenType::Symbol, "}", 7, 42},
        {TokenType::Keyword, "public", 8, 1},
        {TokenType::Keyword, "bool", 8, 8},
        {TokenType::Identifier, "do_work", 8, 13},
        {TokenType::Symbol, "(", 8, 20},
        {TokenType::Keyword, "string", 8, 21},
        {TokenType::Identifier, "param1", 8, 28},
        {TokenType::Symbol, ",", 8, 34},
        {TokenType::Keyword, "double", 8, 36},
        {TokenType::Identifier, "param2", 8, 43},
        {TokenType::Symbol, ",", 8, 49},
        {TokenType::Identifier, "YetAnotherClass", 8, 51},
        {TokenType::Identifier, "param3", 8, 67},
        {TokenType::Symbol, ")", 8, 73},
        {TokenType::Symbol, "{", 9, 1},
        {TokenType::Symbol, "}", 10, 1},
        {TokenType::Keyword, "public", 11, 1},
        {TokenType::Keyword, "override", 11, 8},
        {TokenType::Keyword, "bool", 11, 17},
        {TokenType::Identifier, "Equals", 11, 22},
        {TokenType::Symbol, "(", 11, 28},
        {TokenType::Identifier, "Object", 11, 29},
        {TokenType::Identifier, "other", 11, 36},
        {TokenType::Symbol, ")", 11, 42},
        {TokenType::Symbol, "{", 12, 1},
        {TokenType::Symbol, "}", 13, 1},
        {TokenType::Symbol, "}", 14, 1}};

    ASSERT_EQ(symbol_table.size(), expect_symbol_table.size())
        << "Token count mismatch.";

    for (size_t i = 0; i < symbol_table.size(); ++i) {
      const Token &tb = symbol_table[i];
      const Token &sl = expect_symbol_table[i];

      EXPECT_EQ(tb.type, sl.type) << "Mismatch at token #" << i << " (type)";
      EXPECT_EQ(tb.value, sl.value) << "Mismatch at token #" << i << " (value)";

      // Lines and Columns are not super important right now, check later
      // EXPECT_EQ(tb.line, sl.line) << "Mismatch at token #" << i << " (line)";
      // EXPECT_EQ(tb.column, sl.column) << "Mismatch at token #" << i << "
      // (column)";
    }
  } catch (const std::exception &e) {
    FAIL() << e.what();
  } catch (...) {
    FAIL() << "Unexpected unknown exception thrown.";
  }
}

TEST(TokenTableTests, NormalTable2) {
  try {
    std::string file_contents = R"(public class Person
  {
    public string Name { get; set; }
    public int Age { get; set; }

    public Person(string name, int age)
    {
      Name = name;
      Age = age;
    }

    public void SayHello()
    {
      Console.WriteLine("Hello, " + Name);
    }
  })";

    std::istringstream is(file_contents);
    std::vector<Token> symbol_table = Tokenizer::construct_symbol_table(&is);

    std::vector<Token> expected = {
        {TokenType::Keyword, "public", 1, 1},
        {TokenType::Keyword, "class", 1, 10},
        {TokenType::Identifier, "Person", 1, 16},
        {TokenType::Symbol, "{", 2, 3},
        {TokenType::Keyword, "public", 3, 5},
        {TokenType::Keyword, "string", 3, 12},
        {TokenType::Identifier, "Name", 3, 19},
        {TokenType::Symbol, "{", 3, 24},
        {TokenType::Keyword, "get", 3, 26},
        {TokenType::Symbol, ";", 3, 29},
        {TokenType::Keyword, "set", 3, 31},
        {TokenType::Symbol, ";", 3, 34},
        {TokenType::Symbol, "}", 3, 35},
        {TokenType::Keyword, "public", 4, 5},
        {TokenType::Keyword, "int", 4, 12},
        {TokenType::Identifier, "Age", 4, 16},
        {TokenType::Symbol, "{", 4, 20},
        {TokenType::Keyword, "get", 4, 22},
        {TokenType::Symbol, ";", 4, 25},
        {TokenType::Keyword, "set", 4, 27},
        {TokenType::Symbol, ";", 4, 30},
        {TokenType::Symbol, "}", 4, 31},
        {TokenType::Keyword, "public", 5, 5},
        {TokenType::Identifier, "Person", 5, 12},
        {TokenType::Symbol, "(", 5, 18},
        {TokenType::Keyword, "string", 5, 19},
        {TokenType::Identifier, "name", 5, 26},
        {TokenType::Symbol, ",", 5, 30},
        {TokenType::Keyword, "int", 5, 32},
        {TokenType::Identifier, "age", 5, 36},
        {TokenType::Symbol, ")", 5, 39},
        {TokenType::Symbol, "{", 5, 5},
        {TokenType::Symbol, "}", 8, 5},
        {TokenType::Keyword, "public", 10, 5},
        {TokenType::Keyword, "void", 11, 12},
        {TokenType::Identifier, "SayHello", 11, 17},
        {TokenType::Symbol, "(", 11, 25},
        {TokenType::Symbol, ")", 11, 26},
        {TokenType::Symbol, "{", 12, 5},
        {TokenType::Symbol, "}", 14, 3},
        {TokenType::Symbol, "}", 15, 1},
    };

    ASSERT_EQ(symbol_table.size(), expected.size()) << "Token count mismatch.";

    for (size_t i = 0; i < symbol_table.size(); ++i) {
      const Token &tb = symbol_table[i];
      const Token &exp = expected[i];

      EXPECT_EQ(tb.type, exp.type) << "Mismatch at token #" << i << " (type)";
      EXPECT_EQ(tb.value, exp.value)
          << "Mismatch at token #" << i << " (value)";
    }
  } catch (const std::exception &e) {
    FAIL() << e.what();
  } catch (...) {
    FAIL() << "Unexpected unknown exception thrown.";
  }
}
