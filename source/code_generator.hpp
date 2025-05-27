#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include "parser.hpp"
#include "tokenizer.hpp"
#include <fstream>
#include <string>

class CodeGenerator {
public:
  static void generate_header(const ClassNode &class_node, std::ofstream &of);
  static void generate_source(const ClassNode &class_node, std::ofstream &of);

private:
  static std::string generate_modifier_string(AccessModifier access);
  static std::string generate_field(const FieldNode &field);
  static std::string generate_method_declaration(const MethodNode &method,
                                                 const std::string &class_name);
  static std::string generate_method_definition(const MethodNode &method,
                                                const std::string &class_name);
  static std::string
  generate_property_declarations(const PropertyNode &property);
  static std::string
  generate_property_definitions(const PropertyNode &property,
                                const std::string &class_name);
  static std::string
  generate_constructor_declaration(const std::string &class_name);
  static std::string
  generate_constructor_definition(const std::string &class_name);
  static std::string
  generate_destructor_declaration(const std::string &class_name);
  static std::string
  generate_destructor_definition(const std::string &class_name);
  static std::string generate_equals_declaration(const std::string &class_name);
  static std::string generate_equals_definition(const std::string &class_name);
  static std::string
  generate_param_list(const std::vector<MethodParam> &params);
};

#endif