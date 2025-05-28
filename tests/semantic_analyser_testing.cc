#include "../source/code_generator.hpp"
#include "../source/custom_exceptions.hpp"
#include "../source/file_handler.hpp"
#include "../source/lexer.hpp"
#include "../source/parser.hpp"
#include "../source/validator.hpp"
#include <gtest/gtest.h>

// Helper functions to construct nodes with necessary fields

FieldNode make_field(const std::string &name, const std::string &type = "int") {
  FieldNode f;
  f.name = name;
  f.type = type;
  return f;
}

PropertyNode make_property(const std::string &name,
                           const std::string &type = "int") {
  PropertyNode p;
  p.name = name;
  p.type = type;
  // empty accessors for simplicity
  return p;
}

MethodParam make_param(const std::string &type, const std::string &name) {
  return MethodParam{type, name};
}

MethodNode make_method(const std::string &name,
                       const std::vector<MethodParam> &params = {}) {
  MethodNode m;
  m.name = name;
  m.parameters = params;
  return m;
}

ClassNode make_class(const std::string &name,
                     std::optional<std::string> base_class = std::nullopt) {
  ClassNode c;
  c.name = name;
  c.base_class = base_class;
  return c;
}

// --- TESTS ---

TEST(ValidatorTests, DuplicateFieldsThrows) {
  ClassNode c = make_class("MyClass");
  c.fields.push_back(make_field("field1"));
  c.fields.push_back(make_field("field1")); // duplicate

  EXPECT_THROW(Validator::ensure_no_field_duplicate_within_class(c),
               Validator_Exception);
}

TEST(ValidatorTests, DuplicatePropertiesThrows) {
  ClassNode c = make_class("MyClass");
  c.properties.push_back(make_property("prop1"));
  c.properties.push_back(make_property("prop1")); // duplicate

  EXPECT_THROW(Validator::ensure_no_property_duplicate_within_class(c),
               Validator_Exception);
}

TEST(ValidatorTests, DuplicateMethodsThrows) {
  ClassNode c = make_class("MyClass");

  MethodParam p1 = make_param("int", "a");
  MethodParam p2 = make_param("double", "b");

  c.methods.push_back(make_method("foo", {p1, p2}));
  c.methods.push_back(
      make_method("foo", {p1, p2})); // exact same param types -> duplicate

  EXPECT_THROW(Validator::ensure_no_method_duplicate_within_class(c),
               Validator_Exception);
}

TEST(ValidatorTests, MethodsWithSameNameDifferentParamsDoesNotThrow) {
  ClassNode c = make_class("MyClass");

  MethodParam p1 = make_param("int", "a");
  MethodParam p2 = make_param("double", "b");

  c.methods.push_back(make_method("foo", {p1}));
  c.methods.push_back(
      make_method("foo", {p2})); // different param types, no error

  EXPECT_NO_THROW(Validator::ensure_no_method_duplicate_within_class(c));
}

TEST(ValidatorTests, ValidClassHierarchyDoesNotThrow) {
  ClassNode base = make_class("Base");
  ClassNode derived = make_class("Derived", "Base");

  std::vector<ClassNode> classes = {base, derived};

  EXPECT_NO_THROW(Validator::ensure_class_hierarchy(classes));
}

TEST(ValidatorTests, UndefinedBaseClassThrows) {
  ClassNode derived = make_class("Derived", "NonExistentBase");

  std::vector<ClassNode> classes = {derived};

  EXPECT_THROW(Validator::ensure_class_hierarchy(classes), Validator_Exception);
}

TEST(ValidatorTests, EnsureValidStructureThrowsOnDuplicates) {
  ClassNode c = make_class("MyClass");
  c.fields.push_back(make_field("field1"));
  c.fields.push_back(make_field("field1")); // duplicate field

  std::vector<ClassNode> classes = {c};

  EXPECT_THROW(Validator::ensure_valid_structure(classes), Validator_Exception);
}

TEST(ValidatorTests, EnsureValidStructureNoThrowForValidInput) {
  ClassNode c = make_class("MyClass");
  c.fields.push_back(make_field("field1"));
  c.properties.push_back(make_property("prop1"));

  MethodParam p1 = make_param("int", "x");
  c.methods.push_back(make_method("foo", {p1}));

  std::vector<ClassNode> classes = {c};

  EXPECT_NO_THROW(Validator::ensure_valid_structure(classes));
}
