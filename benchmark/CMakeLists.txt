cmake_minimum_required(VERSION 3.20)
project(ufmt-benchmark)

set(CMAKE_CXX_STANDARD 17)

add_executable(ufmt-benchmark
        benchmark.cpp
        ../thirdparty/source/format.cc)

target_link_libraries(ufmt-benchmark PRIVATE ufmt)
target_include_directories(ufmt-benchmark PRIVATE
        ../thirdparty/include)

