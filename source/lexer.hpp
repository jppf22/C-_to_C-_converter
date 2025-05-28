#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>

enum class TokenType { Keyword, Identifier, Symbol, EndOfFile };

struct Token {
  TokenType type;
  std::string value;
};

class Lexer {
public:
  Lexer(std::istream *input);
  Token next_token();
  Token post_skip();
  Token peek_token();

  void falsify_peek_flag();
  bool has_more_tokens();

  int get_line();
  int get_line_before_identifier_or_keyword();
  int get_column();
  int get_column_before_identifier_or_keyword();
  char get_current_char();
  void skipBracedBlock();

private:
  std::istream *input;
  char current_char;
  bool has_peeked;
  Token peeked_token;

  int line = 1;
  int column = 0;
  int line_before_identifier_or_keyword = 1;
  int column_before_identifier_or_keyword = 0;

  void advance();
  void skip_whitespace();
  Token next_token_internal();

  Token parse_identifier_or_keyword();
  Token parse_symbol();
  bool is_keyword(const std::string &word);
  bool is_recognize_symbol(const char &symbol);
};
#endif