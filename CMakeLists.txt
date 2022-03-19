cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(ufmt)

add_library(ufmt INTERFACE
    include/ufmt/text.hpp
    include/ufmt/print.hpp
    include/ufmt/buffered_file.hpp
    include/ufmt/text_file.hpp)

target_include_directories(ufmt INTERFACE include)

add_subdirectory(test)
add_subdirectory(benchmark)

