cmake_minimum_required(VERSION 3.14)
project(my_project)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# Source files shared by both tests
set(SOURCE_FILES
  source/lexer.cpp
  source/file_handler.cpp
  source/parser.cpp
  source/code_generator.cpp
  source/validator.cpp
)


add_executable(full_testing tests/full_testing.cc ${SOURCE_FILES})
target_include_directories(full_testing PRIVATE source)
target_link_libraries(full_testing GTest::gtest_main)
gtest_discover_tests(full_testing)


add_executable(semantic_analyser_testing tests/semantic_analyser_testing.cc ${SOURCE_FILES})
target_include_directories(semantic_analyser_testing PRIVATE source)
target_link_libraries(semantic_analyser_testing GTest::gtest_main)
gtest_discover_tests(semantic_analyser_testing)

add_executable(syntax_analyser_testing tests/syntax_analyser_testing.cc ${SOURCE_FILES})
target_include_directories(syntax_analyser_testing PRIVATE source)
target_link_libraries(syntax_analyser_testing GTest::gtest_main)
gtest_discover_tests(syntax_analyser_testing)

target_compile_definitions(full_testing PRIVATE TEST_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")

# Copy test inputs and outputs after build
add_custom_command(TARGET full_testing POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/inputs
    ${CMAKE_CURRENT_BINARY_DIR}/tests/inputs
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/outputs
    ${CMAKE_CURRENT_BINARY_DIR}/tests/outputs
)
