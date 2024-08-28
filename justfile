project := "ufmt"
test-file := project + "-test"
bench-file := project + "-bench"
flags := "-std=c++20 -Iinclude -Ithirdparty/include"
debug-flags := flags + " -g -O0"
release-flags := flags + " -O3 -DNDEBUG"

alias b := build

default: test

build-test:
    mkdir -p build
    c++ test/test.cpp -o build/{{test-file}} {{debug-flags}}

build-bench:
    mkdir -p build
    c++ benchmark/benchmark.cpp -o build/{{bench-file}} {{release-flags}}

build: build-test build-bench

test: build-test
    build/{{test-file}}

bench: build-bench
    build/{{bench-file}}

clean:
    rm -rf build

