#include "tokenizer.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <vector>

#include "custom_exceptions.hpp"
#include "file_handler.hpp"

std::array<char, 13> recognised_symbol_list = {':', ';', ',', '.', '=', '{',
                                               '}', '(', ')', '[', ']'};

std::array<std::string, 16> recognised_keyword_list = {
    "class",    "public", "private", "protected", "static", "virtual",
    "override", "new",    "void",    "int",       "string", "bool",
    "float",    "double", "get",     "set"};

std::vector<Token> construct_symbol_table(std::istream *is) {
  std::vector<Token> symbol_table;

  int line = 1;
  int column = 0;

  char c;
  int c_as_int;
  bool in_method_body = false;
  int brace_depth = 0;

  while ((c_as_int = get_input_stream_char(is)) != EOF) {

    c = static_cast<char>(c_as_int);

    if (c == '\n') {
      line++;
      column = 0;
      continue;
    }

    column++;

    if (in_method_body) {
      if (c == '}') {
        brace_depth--;
        if (brace_depth == 0) {
          in_method_body =
              false; // finished skipping method body, stop ignoring
        }
      } else {

        if (c == '{') {
          brace_depth++;
        }

        continue;
      }
    }

    if (std::isspace(c_as_int)) {
      continue;
    } else if (std::isalpha(c_as_int) || c == '_') {
      // start tracking identifier

      int start_column = column;
      TokenType type = TokenType::Identifier;

      std::string identifier;
      do {
        identifier += c;
        c_as_int = get_input_stream_char(is);
        c = static_cast<char>(c_as_int);
        column++;
      } while ((std::isalnum(c_as_int) || c == '_') && c != EOF);

      column--;
      is->putback(c);

      // TODO: Make it more efficient by removing possible options
      for (const auto &kw : recognised_keyword_list) {
        if (identifier == kw) {
          type = TokenType::Keyword;
          break;
        }
      }

      symbol_table.push_back(Token{type, identifier, line, start_column});

    } else if (std::isdigit(c_as_int)) {
      /*
      DOUBT:
      - if not ignoring insides of methods -> must no be considered error and
      additional logic must be added
      - (CURRENT APPROACH) if ignoring insides of methods -> can consider error
      for number to appear outside of a method (Note: assuming C# program is
      compilable this never happens)
      */
      throw Tokenizer_Exception(
          "Identifier starting with number or number outside of method");
    } else if (is_recognised_symbol(c)) {
      int symbol_col = column;

      // Check if it's the start of a method body
      if (c == '{' && !symbol_table.empty() &&
          symbol_table.back().value == ")") {
        in_method_body = true;
        brace_depth = 1;
      }

      // Store the actual symbol (not the lookahead character!)
      symbol_table.push_back(
          Token{TokenType::Symbol, std::string(1, c), line, symbol_col});
    }
  }

  return symbol_table;
}

bool is_recognised_symbol(char c) {
  return (std::find(recognised_symbol_list.begin(),
                    recognised_symbol_list.end(),
                    c) != recognised_symbol_list.end());
}