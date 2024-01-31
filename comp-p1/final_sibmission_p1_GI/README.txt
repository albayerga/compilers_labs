To run code, first you need to build preprocessor.c and then run with desired arguments.

Execution structure:

preprocessor [flags] <program.c>


CMakeFile cLion example:

cmake_minimum_required(VERSION 3.21)
project(untitled1 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled1 preprocessor.c)