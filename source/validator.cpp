#include "validator.hpp"
#include "custom_exceptions.hpp"
#include <unordered_set>

void Validator::ensure_valid_structure(std::vector<ClassNode> &classes) {
  for (const auto &specific_class : classes) {
    ensure_no_field_duplicate_within_class(specific_class);
    ensure_no_property_duplicate_within_class(specific_class);
    ensure_no_method_duplicate_within_class(specific_class);
  }

  ensure_class_hierarchy(classes);
}

void Validator::ensure_no_field_duplicate_within_class(
    ClassNode specific_class) {
  std::unordered_set<std::string> seen_names;

  for (const auto &field : specific_class.fields) {
    if (seen_names.find(field.name) != seen_names.end()) {
      std::string message =
          "Duplicate field: " + field.name + " in class " + specific_class.name;
      throw Validator_Exception(message.c_str());
    }

    seen_names.insert(field.name);
  }
}

void Validator::ensure_no_property_duplicate_within_class(
    ClassNode specific_class) {
  std::unordered_set<std::string> seen_properties;

  for (const auto &property : specific_class.properties) {

    if (seen_properties.find(property.name) != seen_properties.end()) {
      std::string message = "Duplicate property: " + property.name +
                            " in class " + specific_class.name;
      throw Validator_Exception(message.c_str());
    }

    seen_properties.insert(property.name);
  }
}

bool have_same_parameter_list(const MethodNode &a, const MethodNode &b) {
  if (a.parameters.size() != b.parameters.size()) {
    return false;
  }
  for (size_t i = 0; i < a.parameters.size(); ++i) {
    if (a.parameters[i].type != b.parameters[i].type) {
      return false;
    }
  }
  return true;
}

void Validator::ensure_no_method_duplicate_within_class(
    ClassNode specific_class) {
  std::vector<MethodNode> seen_methods;

  for (const MethodNode &method : specific_class.methods) {
    for (const MethodNode &existing : seen_methods) {
      if (method.name == existing.name &&
          have_same_parameter_list(method, existing)) {
        std::string message = "Duplicate method: '" + method.name +
                              "' with same parameter types in class '" +
                              specific_class.name + "'";
        throw Validator_Exception(message.c_str());
      }
    }
    seen_methods.push_back(method);
  }
}

void Validator::ensure_class_hierarchy(std::vector<ClassNode> classes) {
  std::unordered_set<std::string> defined_class_names;
  for (const auto &specific_class : classes) {
    defined_class_names.insert(specific_class.name);
  } // Assumes only necessity for base class to be in same file, not necessarily
    // before definition of child class

  for (const auto &specific_class : classes) {
    if (specific_class.base_class) {
      if (defined_class_names.find(*specific_class.base_class) ==
          defined_class_names.end()) {
        std::string message = "Undefined base class '" +
                              *specific_class.base_class + "' for class " +
                              specific_class.name;
        throw Validator_Exception(message.c_str());
      }
    }
  }
}