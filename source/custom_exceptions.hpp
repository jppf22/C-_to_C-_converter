#ifndef CUSTOM_EXCEPTIONS
#define CUSTOM_EXCEPTIONS

#include <exception>
#include <string>

// TODO: ADD MECHANISM FOR LINE AND COLUMN SHOWCASE, AND LOCATE LINE WITHIN
// SOURCE CODE

class IO_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  IO_Exception(const char *msg)
      : message(msg), full_message("IO_Exception: " + message) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

class Tokenizer_Exception : public std::exception {
private:
  std::string message;
  std::string full_message;

public:
  Tokenizer_Exception(const char *msg)
      : message(msg), full_message("Tokenizer_Exception: " + message) {}

  const char *what() const noexcept override { return full_message.c_str(); }
};

#endif // CUSTOM_EXCEPTIONS