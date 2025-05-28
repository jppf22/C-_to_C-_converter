#include "parser.hpp"
#include "custom_exceptions.hpp"
#include <iostream>
#include <sstream>

// Builder and navigation operations -----------------------------------

Parser::Parser(Lexer &_lexer) : lexer(_lexer) {
  current_token = lexer.next_token();
  last_token = current_token;
}

bool Parser::isLastToken() {
  return current_token.type == TokenType::EndOfFile;
}

bool Parser::match(const std::string &value) {

  // std::cout << "CURRENT: " << current_token.value << "\tMATCH: " << value <<
  // '\n';

  if (current_token.value == value) {
    last_token = current_token;
    current_token = lexer.next_token();
    return true;
  }
  return false;
}

bool Parser::match(TokenType type) {

  if (current_token.type == type) {
    last_token = current_token;
    current_token = lexer.next_token();
    return true;
  }
  return false;
}

void Parser::expectTokenValue(const std::string &value) {
  if (!match(value)) {
    throw Parser_Exception(("Expected token '" + value + "'").c_str(),
                           lexer.get_line(), lexer.get_column());
  }
}

bool Parser::isNextTokenEqualTo(const std::string &value) {
  return current_token.value == value;
}

// Parsing functions -----------------------------------

std::vector<ClassNode> Parser::parseProgram() {
  std::vector<ClassNode> classes;
  while (!isLastToken()) {
    classes.push_back(parseClassDeclaration());
  }
  return classes;
}

ClassNode Parser::parseClassDeclaration() {
  ClassNode class_node;
  class_node.access = tryParseAccessModifier();
  expectTokenValue("class");
  class_node.name = parseIdentifier();
  class_node.base_class = tryParseBaseClass();
  expectTokenValue("{");
  parseMemberDeclarations(class_node);
  expectTokenValue("}");
  return class_node;
}

std::string Parser::parseIdentifier() {
  if (!match(TokenType::Identifier)) {
    std::ostringstream oss;
    oss << "Expected Identifier at char \'" << lexer.get_current_char() << '\'';
    throw Parser_Exception(oss.str().c_str(), lexer.get_line(),
                           lexer.get_column());
  }
  return last_token.value;
}

std::optional<std::string> Parser::tryParseBaseClass() {
  if (match(":")) {
    return parseIdentifier();
  }
  return std::nullopt;
}

void Parser::parseMemberDeclarations(ClassNode &classNode) {
  while (!isNextTokenEqualTo("}") && !isLastToken()) {
    auto accessModifier = tryParseAccessModifier();
    bool is_override = tryParseMethodOverride();

    if (!isLastToken() && current_token.type == TokenType::Identifier &&
        current_token.value == classNode.name &&
        lexer.peek_token().value == "(") { // Member is a constructor

      std::string identifier = parseIdentifier(); // class name (constructor)
      match("(");
      std::vector<MethodParam> parameters;
      int n_parameters = 0;
      while (!isNextTokenEqualTo(")")) {
        if (n_parameters > 0) {
          expectTokenValue(",");
        }
        std::string type = parseType();
        std::string name = parseIdentifier();
        parameters.push_back(MethodParam{type, name});
        n_parameters++;
      }
      expectTokenValue(")");
      expectTokenValue("{");
      skipMethodBody();

      classNode.methods.push_back(MethodNode{
          accessModifier,
          std::nullopt, // no return type for constructor
          identifier, parameters,
          false, // is_override
          true   // is_constructor
      });
    } else {

      std::string type = parseType();
      std::string identifier = parseIdentifier();

      if (match(";")) { // Member is a Field
        classNode.fields.push_back(FieldNode{accessModifier, type, identifier});
      } else if (match("(")) { // Member is a Method
        std::vector<MethodParam> parameters;
        int n_parameters = 0;
        while (!isNextTokenEqualTo(")")) {
          if (n_parameters > 0) {
            expectTokenValue(",");
          }
          std::string type = parseType();
          std::string name = parseIdentifier();
          parameters.push_back(MethodParam{type, name});
          n_parameters++;
        }
        expectTokenValue(")");
        expectTokenValue("{");
        skipMethodBody();

        std::optional<std::string> opt_type = type;

        classNode.methods.push_back(MethodNode{
            accessModifier, opt_type, identifier, parameters, is_override});

      } else if (match("{")) { // Member is a Property
        std::vector<PropertyAcessor> accessors;
        int n_accesors = 0;
        while (!isNextTokenEqualTo("}")) {

          PropertyAcessor accessor;
          if (match("get")) {
            accessor.operation = "get";
          } else if (match("set")) {
            accessor.operation = "set";
          } else {
            throw Parser_Exception(
                "Unsupported acessor operation - only \'get\' "
                "and \'set\' are currently supported",
                lexer.get_line(), lexer.get_column());
          }

          if (match("{")) {
            std::cout << current_token.value << " " << last_token.value << "\n";
            skipMethodBody();
            accessor.has_brackets = true;
          }

          expectTokenValue(";");

          accessors.push_back(accessor);
          n_accesors++;
        }
        expectTokenValue("}");

        classNode.properties.push_back(
            PropertyNode{accessModifier, type, identifier, accessors});
      } else {
        throw Parser_Exception("Expected Member Declaration", lexer.get_line(),
                               lexer.get_column());
      }
    }
  }
}

std::string Parser::parseType() {
  if (match("void") || match("int") || match("float") || match("double") ||
      match("bool") || match("string") || match(TokenType::Identifier)) {
    return last_token.value;
  }
  throw Parser_Exception("Expected a Type", lexer.get_line(),
                         lexer.get_column());
}

AccessModifier Parser::parseAccessModifier() {
  auto modifier = tryParseAccessModifier();
  if (!modifier)
    throw Parser_Exception("Expected Access Modifier", lexer.get_line(),
                           lexer.get_column());
  return *modifier;
}

std::optional<AccessModifier> Parser::tryParseAccessModifier() {
  if (match("public"))
    return AccessModifier::Public;
  if (match("private"))
    return AccessModifier::Private;
  if (match("protected"))
    return AccessModifier::Protected;

  return std::nullopt;
}

bool Parser::tryParseMethodOverride() {
  if (match("override")) {
    return true;
  }
  return false;
}

#include <iostream>
#include <optional>
#include <string>
#include <vector>


std::ostream &operator<<(std::ostream &os,
                         const AccessModifier &accessModifier) {
  switch (accessModifier) {
  case AccessModifier::Public:
    os << "public";
    break;
  case AccessModifier::Private:
    os << "private";
    break;
  case AccessModifier::Protected:
    os << "protected";
    break;
  }
  return os;
}


std::ostream &operator<<(std::ostream &os, const MethodParam &methodParam) {
  os << methodParam.type << " " << methodParam.name;
  return os;
}

std::ostream &operator<<(std::ostream &os, const FieldNode &fieldNode) {
  if (fieldNode.access)
    os << *fieldNode.access << " ";
  os << fieldNode.type << " " << fieldNode.name << ";";
  return os;
}

std::ostream &operator<<(std::ostream &os, const MethodNode &methodNode) {
  if (methodNode.access)
    os << *methodNode.access << " ";
  os << *methodNode.return_type << " " << methodNode.name << "(";
  for (size_t i = 0; i < methodNode.parameters.size(); ++i) {
    os << methodNode.parameters[i];
    if (i + 1 < methodNode.parameters.size())
      os << ", ";
  }
  os << ")";
  if (methodNode.is_override)
    os << " override";
  os << " {}"; 
  return os;
}

std::ostream &operator<<(std::ostream &os, const PropertyAcessor &accessor) {
  os << accessor.operation;
  if (accessor.has_brackets)
    os << " {}";
  return os;
}


std::ostream &operator<<(std::ostream &os, const PropertyNode &propertyNode) {
  if (propertyNode.access)
    os << *propertyNode.access << " ";
  os << propertyNode.type << " " << propertyNode.name << " { ";
  for (const auto &accessor : propertyNode.accessors) {
    os << accessor << " ";
  }
  os << "}";
  return os;
}

std::ostream &operator<<(std::ostream &os, const ClassNode &classNode) {
  if (classNode.access)
    os << *classNode.access << " ";
  os << "class " << classNode.name;
  if (classNode.base_class)
    os << " : " << *classNode.base_class;
  os << " {\n\n";

  for (const auto &field : classNode.fields) {
    os << "  " << field << "\n";
  }
  for (const auto &property : classNode.properties) {
    os << "  " << property << "\n";
  }
  for (const auto &method : classNode.methods) {
    os << "  " << method << "\n";
  }

  os << "};";
  return os;
}

void Parser::skipMethodBody() {

  if (current_token.value == "}") {
    last_token = current_token;
    current_token = lexer.next_token();
    return;
  }

  lexer.skipBracedBlock();
  last_token = Token{TokenType::Symbol, "}"};
  current_token = lexer.next_token();
}