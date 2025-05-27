#include "tokenizer.hpp"
#include <optional>
#include <string>
#include <vector>

#ifndef PARSER
#define PARSER

/* Missing more optional token parsing:
- Optional Member List
- Static modifiers
- Const modifiers
- Optional Parameter List
- Optional getters and setters
*/

enum class AccessModifier { Public, Private, Protected };

struct FieldNode {
  std::optional<AccessModifier> access;
  std::string type;
  std::string name;
  // bool is_static = false;
};

struct MethodParam {
  std::string type;
  std::string name;
};

struct MethodNode {
  std::optional<AccessModifier> access;
  std::optional<std::string> return_type;
  std::string name;
  std::vector<MethodParam> parameters;
  bool is_override = false;
  bool is_constructor = false;
  // bool is_static = false;
};

struct PropertyAcessor {
  std::string operation;
  bool has_brackets = false;
};

struct PropertyNode {
  std::optional<AccessModifier> access;
  std::string type;
  std::string name;
  std::vector<PropertyAcessor> accessors;
};

struct ClassNode {
  std::string name;
  std::optional<std::string> base_class;
  std::optional<AccessModifier> access;
  std::vector<FieldNode> fields;
  std::vector<MethodNode> methods;
  std::vector<PropertyNode> properties;
};

class Parser {
private:
  std::vector<Token> symbol_table;
  int num_symbol;
  int current_symbol = 0;

public:
  Parser(std::vector<Token> symbol_table);
  std::vector<ClassNode> parseProgram();
  ClassNode parseClassDeclaration();
  std::optional<std::string> tryParseBaseClass();
  void parseMemberDeclarations(ClassNode &classNode);
  std::string parseType();
  std::string parseIdentifier();
  AccessModifier parseAccessModifier();
  std::optional<AccessModifier> tryParseAccessModifier();
  bool tryParseMethodOverride();

  bool isLastToken();
  bool match(std::string str);
  bool match(TokenType token_type);
  void expectTokenValue(std::string tokenValue);
  bool isNextTokenEqualTo(std::string tokenValue);
};

std::ostream &operator<<(std::ostream &os, const ClassNode &classNode);
std::ostream &operator<<(std::ostream &os, const PropertyNode &propertyNode);
std::ostream &operator<<(std::ostream &os, const MethodNode &methodNode);
std::ostream &operator<<(std::ostream &os, const FieldNode &fieldNode);
std::ostream &operator<<(std::ostream &os, const MethodParam &methodParam);
std::ostream &operator<<(std::ostream &os,
                         const AccessModifier &accessModifier);

#endif