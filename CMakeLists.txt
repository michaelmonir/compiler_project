cmake_minimum_required(VERSION 3.23++)
project(compiler_project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main project executable
add_executable(compiler_project src/main.cpp src/RulesParser/RulesParser.h src/RulesParser/RulesParser.cpp src/RulesParser/RegularExpression.h src/RulesParser/RegularDefinition.h src/RulesParser/stringUtils.cpp src/RulesParser/TokenType.h)

# Add Google Test from the external/gtest directory
add_subdirectory(external/gtest)

# Enable testing
enable_testing()

# Add test executable
add_executable(runTests 
    test/try_gtest/try_gtest_test.cpp
    test/dfa_conversion/dfa_convertor_test.cpp
    src/try_gtest/try_gtest.cpp
    src/dfa_conversion/dfa_convertor.cpp
)

# Link Google Test to the test executable
target_link_libraries(runTests PRIVATE gtest gtest_main)

target_include_directories(runTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/try_gtest
    ${CMAKE_SOURCE_DIR}/src/dfa_conversion
)

# Include directories for tests
target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add a test for CTest
add_test(NAME TryGTestTest COMMAND runTests)

include(GoogleTest)
gtest_discover_tests(runTests)