#ifndef VALIDATOR
#define VALIDATOR

#include "parser.hpp"
#include <vector>

// TODO: Check if override actually overrides something (Verify all methods
// inherited and that override is equal to the signature of any)

// TODO: verify user-made datatypes actually exist

// TODO: verify no duplication between properties and fields

class Validator {
public:
  static void ensure_valid_structure(std::vector<ClassNode> &classes);

  static void ensure_no_field_duplicate_within_class(ClassNode specific_class);
  static void
  ensure_no_property_duplicate_within_class(ClassNode specific_class);
  static void ensure_no_method_duplicate_within_class(ClassNode specific_class);
  static void ensure_class_hierarchy(std::vector<ClassNode> classes);
};

#endif