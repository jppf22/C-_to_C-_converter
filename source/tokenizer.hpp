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

class Tokenizer {
public:
  static std::vector<Token> construct_symbol_table(std::istream *is);
  static bool is_recognised_symbol(char c);
};

std::ostream &operator<<(std::ostream &os, const Token &token);

#endif