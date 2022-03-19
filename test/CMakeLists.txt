cmake_minimum_required(VERSION 3.20)
project(ufmt-test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ufmt-test
        test.cpp
        text.hpp
        doctest.h)

target_link_libraries(ufmt-test PRIVATE ufmt)

