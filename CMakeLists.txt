cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(
  semantic_analyser_testing
  semantic_analyser_testing.cc

  source/lexer.cpp
  source/lexer.hpp
  source/file_handler.hpp
  source/file_handler.cpp
  source/parser.hpp
  source/parser.cpp
  source/code_generator.cpp
  source/code_generator.hpp
  source/validator.cpp
  source/validator.hpp
)

target_link_libraries(semantic_analyser_testing GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(semantic_analyser_testing)
