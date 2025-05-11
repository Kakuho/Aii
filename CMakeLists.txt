cmake_minimum_required(VERSION 3.1...3.29)

project(Aii VERSION 1.0 
            LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

option(DEBUG "Debug Enable" OFF)
if(DEBUG)
  message("Compile With Debugging On")
  add_compile_options(-g -O0)
endif(DEBUG)

include_directories(include)
include_directories(extern)

add_subdirectory(tests)
add_subdirectory(examples)
