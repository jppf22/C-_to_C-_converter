#include "code_generator.hpp"
#include <sstream>
#include <unordered_map>

#define METHOD_COMMENT "//TODO: implement this method"

// Writes the class info into the header file
void CodeGenerator::generate_header(const ClassNode &class_node,
                                    std::ostream &of) {
  of << "class " << class_node.name;
  if (class_node.base_class) {
    of << " : public " << *class_node.base_class;
  }
  of << " {\n";

  // Generate the contents for the public:, private:, protected: blocks
  std::unordered_map<AccessModifier, std::vector<std::string>> blocks;

  // Public: Constructor
  blocks[AccessModifier::Public].push_back(
      generate_constructor_declaration(class_node.name));

  // Fields - Go into each block according to C# structure.
  for (const FieldNode &field : class_node.fields) {
    blocks[field.access.value_or(AccessModifier::Private)].push_back(
        generate_field(
            field)); // if access is defined go into that access block, if no
                     // access is defined go into private block by default
  }

  /*
    Properties
    - If Public or not defined: separate into private field and public
    getters/setters
    - If Private or Protected: put both field and getters/setters in same block

  */
  for (const auto &prop : class_node.properties) {
    if ((prop.access.has_value() &&
         prop.access.value() == AccessModifier::Public) ||
        !prop.access.has_value()) {
      FieldNode prop_field_node{AccessModifier::Public, prop.type, prop.name};
      blocks[AccessModifier::Private].push_back(
          generate_field(prop_field_node));
      blocks[AccessModifier::Public].push_back(
          generate_property_declarations(prop));
    } else {
      blocks[prop.access.value()].push_back(
          generate_field(FieldNode{prop.access.value(), prop.type, prop.name}));
      blocks[prop.access.value()].push_back(
          generate_property_declarations(prop));
    }
  }

  for (const auto &method : class_node.methods) {
    if(method.name != class_node.name){
      blocks[method.access.value_or(AccessModifier::Private)].push_back(
          generate_method_declaration(method, class_node.name));
    }
  }

  // Public: Destructor
  blocks[AccessModifier::Public].push_back(
      generate_destructor_declaration(class_node.name));

  // "Print" according to the access modifier blocks
  for (const auto &[access, lines] : blocks) {
    of << generate_modifier_string(access) << ":\n";
    for (const auto &line : lines) {
      of << "    " << line << "\n";
    }
  }

  of << "};\n";
}

// Header file Auxiliary Functions

std::string
CodeGenerator::generate_constructor_declaration(const std::string &class_name) {
  return class_name + "();";
}

std::string
CodeGenerator::generate_destructor_declaration(const std::string &class_name) {
  return "~" + generate_constructor_declaration(class_name);
}

std::string CodeGenerator::generate_field(const FieldNode &field) {
  return format_type(field.type) + " " + trasform_pascal_case_name(field.name) +
         ";";
}

std::string
CodeGenerator::generate_method_declaration(const MethodNode &method,
                                           const std::string &class_name) {
  // EXTRA: Add more special functions
  if (method.name == "Equals") {
    return generate_equals_declaration(class_name);
  } else {

    std::ostringstream oss;

    if (method.return_type.has_value()) {
      oss << format_type(method.return_type.value());
    } else {
      oss << "void";
    }

    oss << " " << trasform_pascal_case_name(method.name) << "(" << generate_param_list(method.parameters)
        << ");";

    return oss.str();
  }
}

std::string
CodeGenerator::generate_equals_declaration(const std::string &class_name) {
  return "bool operator==(const " + class_name + "& other);";
}

std::string
CodeGenerator::generate_param_list(const std::vector<MethodParam> &params) {
  std::ostringstream oss;
  for (size_t i = 0; i < params.size(); i++) {
    oss << format_type(params[i].type) << " " << params[i].name;
    if (i + 1 < params.size())
      oss << ", ";
  }
  return oss.str();
}

std::string
CodeGenerator::generate_property_declarations(const PropertyNode &property) {
  std::ostringstream oss;

  int count = 0;
  for (const auto &accessor : property.accessors) {
    if (count) {
      oss << "\n    ";
    }
    if (accessor.operation == "get") {
      oss << format_type(property.type) << " get_"
          << trasform_pascal_case_name(property.name) << "();";
    } else if (accessor.operation == "set") {
      oss << "void set_" << trasform_pascal_case_name(property.name) << "("
          << format_type(property.type) << " value);";
    }
    count++;
  }

  return oss.str();
}

// -----------------------------------------

// Writes the class info into the source file
void CodeGenerator::generate_source(const ClassNode &class_node,
                                    std::ostream &of) {

  // EXTRA: These are outside project scope, but to ensure validity in case of
  // testing
  of << "#include \"" << class_node.name << ".hpp\"\n";
  // of << "#include <iostream>\n";
  // of << "#include <string>\n";

  of << generate_constructor_definition(class_node.name);

  for (const auto &prop : class_node.properties) {
    of << generate_property_definitions(prop, class_node.name);
  }

  for (const auto &method : class_node.methods) {
    if (method.name == class_node.name)
      continue; // Constructor Already handled beforehand
    of << generate_method_definition(method, class_node.name);
  }

  of << generate_destructor_definition(class_node.name);
}

// Source file Auxiliary Functions

std::string
CodeGenerator::generate_method_definition(const MethodNode &method,
                                          const std::string &class_name) {
  std::ostringstream oss;
  if (method.name == "Equals") {
    oss << generate_equals_definition(class_name);
  } else {

    if (method.return_type) {
      oss << format_type(method.return_type.value());
    } else {
      oss << "void";
    }

    oss << " " << class_name << "::" << trasform_pascal_case_name(method.name) << "("
        << generate_param_list(method.parameters) << ") {\n    "
        << METHOD_COMMENT << "\n}\n";
  }
  return oss.str();
}

std::string
CodeGenerator::generate_property_definitions(const PropertyNode &property,
                                             const std::string &class_name) {
  std::ostringstream oss;

  for (const auto &accessor : property.accessors) {
    if (accessor.operation == "get") {
      oss << format_type(property.type) << " " << class_name << "::get_"
          << trasform_pascal_case_name(property.name) << "() {\n    "
          << METHOD_COMMENT << "\n}\n";
    } else if (accessor.operation == "set") {
      oss << "void " << class_name << "::set_"
          << trasform_pascal_case_name(property.name) << "("
          << format_type(property.type) << " value) {\n    " << METHOD_COMMENT
          << "\n}\n";
    }
  }

  return oss.str();
}

std::string
CodeGenerator::generate_constructor_definition(const std::string &class_name) {
  return class_name + "::" + class_name + "() {\n    " + METHOD_COMMENT +
         "\n}\n";
}

std::string
CodeGenerator::generate_destructor_definition(const std::string &class_name) {
  return class_name + "::~" + class_name + "() {\n    " + METHOD_COMMENT +
         "\n}\n";
}

std::string
CodeGenerator::generate_equals_definition(const std::string &class_name) {
  return "bool " + class_name + "::operator==(const " + class_name +
         "& other) {\n    //TODO: implement this method\n}\n";
}

// Other Auxiliary Functions

std::string CodeGenerator::generate_modifier_string(AccessModifier access) {
  switch (access) {
  case AccessModifier::Public:
    return "public";
  case AccessModifier::Private:
    return "private";
  case AccessModifier::Protected:
    return "protected";
  }
  return "private";
}

std::string CodeGenerator::trasform_snake_case_name(std::string original_name) {
  std::ostringstream oss;
  bool capitalize_next = true;

  for (char ch : original_name) {
    if (ch == '_') {
      capitalize_next = true;
    } else {
      if (capitalize_next) {
        oss << static_cast<char>(std::toupper(ch));
        capitalize_next = false;
      } else {
        oss << ch;
      }
    }
  }

  return oss.str(); // Converts snake_case to PascalCase
}

std::string
CodeGenerator::trasform_pascal_case_name(std::string original_name) {
  std::ostringstream oss;

  for (size_t i = 0; i < original_name.size(); ++i) {
    char ch = original_name[i];
    if (std::isupper(ch)) {
      if (i != 0)
        oss << '_';
      oss << static_cast<char>(std::tolower(ch));
    } else {
      oss << ch;
    }
  }

  return oss.str(); // Converts PascalCase to snake_case
}

std::string CodeGenerator::format_type(const std::string &type) {
  if (type == "string") {
    return "std::string";
  }
  return type;
}