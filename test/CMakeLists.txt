cmake_minimum_required(VERSION 3.20)
project(ufmt-test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ufmt-test
        test.cpp
        text.hpp
        doctest.h
        ../include/ufmt/text.hpp
        ../include/ufmt/print.hpp
        ../include/ufmt/text_file.hpp
        ../include/ufmt/buffered_file.hpp)

target_include_directories(ufmt-test PUBLIC ../include)

add_test(NAME test COMMAND ufmt-test)
enable_testing()
