cmake_minimum_required(VERSION 3.28)
project(compiler_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler_project src/main.cpp)


add_subdirectory(googletest)
enable_testing()