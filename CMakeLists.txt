cmake_minimum_required(VERSION 3.28)
project(compiler_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler_project src/main.cpp)

add_subdirectory(googletest)
enable_testing()

# Add test executable
add_executable(runTests test/test_try_gtest.cpp src/try_gtest.cpp)

# Link Google Test to the test executable
target_link_libraries(runTests PRIVATE gtest gtest_main)

# Add a test
add_test(NAME TryGTestTest COMMAND runTests)