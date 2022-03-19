cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(ufmt)
include(CTest)

add_library(ufmt INTERFACE
    include/ufmt/text.hpp
    include/ufmt/print.hpp
    include/ufmt/buffered_file.hpp
    include/ufmt/text_file.hpp)

add_subdirectory(test)
add_subdirectory(benchmark)

