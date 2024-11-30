cmake_minimum_required(VERSION 3.23)
project(compiler_project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add Google Test from the external/gtest directory
add_subdirectory(external/gtest)

# Main project executable
add_executable(compiler_project
        src/main.cpp
        src/RulesParser/RulesParser.h
        src/RulesParser/RulesParser.cpp
        src/RulesParser/string_to_relation_converter.cpp
        src/RulesParser/string_to_relation_converter.h
)

# Add a test executable
add_executable(runTests
        test/FileParser/string_to_relation_convertor_test.cpp
        test/dfa_conversion/dfa_convertor_test.cpp
        src/dfa_conversion/dfa_convertor.cpp
        src/RulesParser/RulesParser.cpp
)

# Include directories
target_include_directories(runTests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Link Google Test to the test executable
target_link_libraries(runTests PRIVATE gtest gtest_main)

# Automatically discover tests
include(GoogleTest)
gtest_discover_tests(runTests)
