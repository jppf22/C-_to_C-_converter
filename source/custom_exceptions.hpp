#ifndef CUSTOM_EXCEPTIONS
#define CUSTOM_EXCEPTIONS

#include <exception>
#include <iostream>

class IO_Exception : public std::exception {
private:
  std::string message;

public:
  IO_Exception(const char *msg) : message(msg) {};
  const char *what() const throw() {
    return ("IO_Exception: " + message).c_str();
  }
};

class Tokenizer_Exception : public std::exception {
private:
  std::string message;

public:
  Tokenizer_Exception(const char *msg) : message(msg) {};
  const char *what() const throw() {
    return ("Tokenizer_Exception: " + message).c_str();
  }
};

#endif