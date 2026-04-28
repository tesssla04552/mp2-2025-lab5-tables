cmake_minimum_required(VERSION 3.16)
project(Lab_Tables)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Lab_Tables
    src/avl_tree_table.cpp
    src/hash_table.cpp
    src/logger.cpp
    src/main.cpp
    src/polynomials.cpp
    src/sorted_table.cpp
    src/unsorted_table.cpp
)

if(MSVC)
    target_compile_options(Lab5_Tables PRIVATE /W4 /WX)
else()
    target_compile_options(Lab5_Tables PRIVATE -Wall -Wextra -Werror)
endif()

file(GLOB HEADERS "include/*.h")