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
        src/dfa_conversion/dfa_convertor.cpp
        src/dfa_conversion/dfa_convertor.h
        src/program.h
        src/nfa/nfa.cpp
        src/minimal_dfa/minimal_dfa.cpp
        src/minimal_dfa/minimal_dfa.h
        src/lexical_analyzer/lexical_analyzer.h
        src/lexical_analyzer/lexical_analyzer.cpp
)

# Add a test executable
add_executable(runTests
        test/FileParser/string_to_relation_convertor_test.cpp
        test/dfa_conversion/dfa_convertor_test.cpp
        test/minimal_dfa/minimal_dfa_test.cpp
        src/dfa_conversion/dfa_convertor.cpp
        src/RulesParser/RulesParser.cpp
        src/RulesParser/string_to_relation_converter.cpp
        test/program_test.cpp
        test/nfa/nfa_test.cpp
        src/nfa/nfa.cpp  # Add nfa.cpp here!
        src/minimal_dfa/minimal_dfa.cpp
)

# Include directories
target_include_directories(runTests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    src/minimal_dfa/minimal_dfa.cpp
)

# Link Google Test to the test executable
target_link_libraries(runTests PRIVATE gtest gtest_main)

target_include_directories(runTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/try_gtest
    ${CMAKE_SOURCE_DIR}/src/dfa_conversion
    ${CMAKE_SOURCE_DIR}/src/minimal_dfa
)

# Include directories for tests
target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add a test for CTest
add_test(NAME TryGTestTest COMMAND runTests)

include(GoogleTest)
gtest_discover_tests(runTests)
