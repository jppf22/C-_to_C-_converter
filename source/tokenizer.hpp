#ifndef TOKENIZER
#define TOKENIZER

#include <array>
#include <string>
#include <vector>

enum class TokenType { Keyword, Identifier, Symbol };

struct Token {
  TokenType type;
  std::string value;
  int line;
  int column;
};

std::vector<Token> construct_symbol_table(std::istream *is);

extern std::array<char, 13> recognised_symbol_list;
extern std::array<std::string, 16> recognised_keyword_list;

bool is_recognised_symbol(char c);
#endif