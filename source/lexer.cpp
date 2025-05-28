#include "lexer.hpp"
#include "custom_exceptions.hpp"
#include "file_handler.hpp"
#include <cctype>
#include <unordered_set>
#include <sstream>

Lexer::Lexer(std::istream *input)
    : input(input), has_peeked(false), line(1), column(0) {
  advance();
}

void Lexer::advance() {
  current_char = input->get();
  if (current_char == '\n') {
    column = 0;
    line++;
  } else {
    column++;
  }
}

int Lexer::get_line() { return line; }
int Lexer::get_column() { return column; }
char Lexer::get_current_char() { return current_char; }

void Lexer::skip_whitespace() {
  while (std::isspace(current_char)) {
    advance();
  }
}

Token Lexer::peek_token() {
  if (!has_peeked) {
    peeked_token = next_token_internal();
    has_peeked = true;
  }
  return peeked_token;
}

Token Lexer::next_token() {
  if (has_peeked) {
    has_peeked = false;
    return peeked_token;
  }
  return next_token_internal();
}

bool Lexer::has_more_tokens() { return !input->eof(); }

Token Lexer::next_token_internal() {
  skip_whitespace();

  if (std::isalpha(current_char) || current_char == '_') {
    return parse_identifier_or_keyword();
  }

  if (std::isdigit(current_char)) {
    throw Parser_Exception(
        "Identifier starting with number or number outside of method", line,
        column);
  }

  if (input->eof()) {
    return Token{TokenType::EndOfFile, ""};
  }

  return parse_symbol();
}

Token Lexer::parse_identifier_or_keyword() {

  line_before_identifier_or_keyword = line;
  column_before_identifier_or_keyword = column;

  std::string value;
  while (std::isalnum(current_char) || current_char == '_') {
    value += current_char;
    advance();
  }

  if (is_keyword(value)) {
    return Token{TokenType::Keyword, value};
  }
  return Token{TokenType::Identifier, value};
}

Token Lexer::parse_symbol() {

  line_before_identifier_or_keyword = line;
  column_before_identifier_or_keyword = column;
  
  if (!is_recognize_symbol(current_char)) {
    
    std::ostringstream oss;
    oss << "Unrecognized Symbol \'";
    oss << current_char << '\'';
    throw Parser_Exception{oss.str().c_str(), line, column};
  }

  std::string value(1, current_char);
  advance();
  return Token{TokenType::Symbol, value};
}

bool Lexer::is_keyword(const std::string &word) {
  static const std::unordered_set<std::string> keywords = {
      "class",  "public", "private", "protected", "int",      "bool",
      "string", "float",  "get",     "set",       "override", "void"};
  return keywords.find(word) != keywords.end();
}

bool Lexer::is_recognize_symbol(const char &symbol) {
  static const std::unordered_set<char> symbols = {':', ';', ',', '{',
                                                   '}', '(', ')'};
  return symbols.find(symbol) != symbols.end();
}

void Lexer::skipBracedBlock() {
  int braceCount = 1; // Assume '{' already consumed before calling this

  while (braceCount > 0) {
    char c = input->get();
    if (c == '\n') {
      column = 0;
      line++;
    } else {
      column++;
    }

    if (input->eof()) {
      throw Parser_Exception("Unexpected EOF while skipping braced block", line,
                             column);
    }

    if (c == '{') {
      ++braceCount;
    } else if (c == '}') {
      --braceCount;
    }
    // Otherwise just skip the character regardless of what it is
  }

  current_char = input->get();
}

void Lexer::falsify_peek_flag(){
  has_peeked = false;
}

int Lexer::get_line_before_identifier_or_keyword(){return line_before_identifier_or_keyword;}
int Lexer::get_column_before_identifier_or_keyword(){return column_before_identifier_or_keyword;}