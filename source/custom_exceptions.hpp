#ifndef CUSTOM_EXCEPTIONS
#define CUSTOM_EXCEPTIONS

#include <exception>
#include <string>

class IO_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  IO_Exception(const char *msg)
      : message(msg), full_message("IO_Exception: " + message) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

class Parser_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  Parser_Exception(const char *msg, int line, int column)
      : message(msg), full_message("Parser_Exception: " + std::string(msg) +
                                   ". Line: " + std::to_string(line) +
                                   " Column: " + std::to_string(column)) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

class Validator_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  Validator_Exception(const char *msg)
      : message(msg), full_message("Validator_Exception: " + message) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

class Code_Generation_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  Code_Generation_Exception(const char *msg)
      : message(msg), full_message("Code_Generation_Exception: " + message) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

#endif 