project := "ufmt"
test-file := project + "-test"
bench-file := project + "-bench"
# Compiler used to build test/bench binaries. Override with e.g.
#   just cxx="c++" test
cxx := env_var_or_default("CXX", "zig c++")
flags := "-std=c++23 -Iinclude -Ithirdparty/include"
debug-flags := flags + " -g -O0"
release-flags := flags + " -O3 -DNDEBUG"

alias b := build

default: test

build-test:
    mkdir -p build
    {{cxx}} test/test.cpp -o build/{{test-file}} {{debug-flags}}

build-bench:
    mkdir -p build
    {{cxx}} benchmark/benchmark.cpp -o build/{{bench-file}} {{release-flags}}

build: build-test build-bench

test: build-test
    build/{{test-file}}

bench: build-bench
    build/{{bench-file}}

clean:
    rm -rf build

