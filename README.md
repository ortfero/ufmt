# ufmt

Header-only C++20 formatting utilities for building small, fast strings, printing to streams, and emitting compact JSON without external dependencies.

## Features
- Header-only: drop `include/ufmt` into your project—no build system integration required.
- Text builder: stream-style or variadic formatting (`ufmt::text::of`) with width, alignment, quoting, and fixed precision helpers.
- Printing helpers: `ufmt::print` and `ufmt::error_with` for lightweight stdout/stderr output.
- JSON builder: compose JSON objects/arrays without runtime allocations beyond the target string; supports optional fields and custom types.

## Requirements
- C++20 compiler (tested with Clang 15+); standard library only.  
- Optional: [`just`](https://github.com/casey/just) to mirror the development commands below.

## Installation
- Copy the contents of `include/` into your include path, or add this repository as a submodule and include headers via `#include <ufmt/...>`.
- When using vendored fmt headers in `thirdparty/include`, ensure your include path contains both `include` and `thirdparty/include`.

## Quick Start
```cpp
#include <ufmt/print.hpp>
#include <ufmt/json.hpp>

int main() {
    ufmt::print("value: ", 127.562);            // stdout
    auto msg = ufmt::text::of("pi ~= ", ufmt::fixed(3.14159, 2));
    auto json = ufmt::json::of("x", 1, "label", "ok");
    ufmt::error_with(1, "failed: ", json);      // stderr + return code
}
```

## Build, Test, and Bench
- `just test` — build and run doctest suite (`build/ufmt-test`).
- `just bench` — build and run benchmarks (`build/ufmt-bench`) with `-O3 -DNDEBUG`.
- `just build` — build both binaries without executing them.
- `just clean` — remove the `build/` directory.
- Without `just`: see `justfile` for equivalent `c++` invocations; include `-std=c++20 -Iinclude -Ithirdparty/include`.

## Project Layout
- `include/ufmt/*.hpp` — public headers (text, print, JSON utilities, fixed buffers).
- `test/` — doctest suites (`*.test.hpp`) aggregated by `test/test.cpp`.
- `benchmark/` — benchmark harness and `ubench.hpp`.
- `thirdparty/` — vendored dependencies (fmt headers).
- `justfile` — reproducible builds/tests/benchmarks.

## Benchmark Snapshot
Relative numbers on an M2 Pro, Clang 15.0, fmtlib 11.0.3:

| Code                                                                    | Time (ns) | Ratio |
|-------------------------------------------------------------------------|----------:|------:|
| ```text.format('a');```                                                 | 6.0       | x1    |
| ```snprintf(charz, sizeof (charz), "%c", 'a');```                       | 0.7       | x0.1  |
| ```fmt::format_to(charz, "{}", 'a');```                                 | 6.5       | x1.1  |
| ```text.format("some literal");```                                      | 3.8       | x1    |
| ```snprintf(charz, sizeof (charz), "%s", "some literal");```            | 0.3       | x0.1  |
| ```fmt::format_to(charz, "{}", "some literal");```                      | 7.2       | x1.9  |
| ```text.format(-127562);```                                             | 9.8       | x1    |
| ```snprintf(charz, sizeof (charz), "%d", -127562);```                   | 42.0      | x4.3  |
| ```fmt::format_to(charz, "{}", -127562);```                             | 14.5      | x1.5  |
| ```std::to_chars(charz, charz + sizeof charz, -127562);```              | 9.5       | x1    |
| ```text.format(-127562.127562);```                                      | 23.2      | x1    |
| ```snprintf(charz, sizeof (charz), "%f", -127562.127562);```            | 123.6     | x5.3  |
| ```fmt::format_to(charz, "{}", -127562.127562);```                      | 34.2      | x1.5  |
| ```text.print("nums: ", -1, ", ", -2, ", ", -3)```                      | 8.7       | x1    |
| ```snprintf(charz, sizeof (charz), "nums: %d, %d, %d", -1, -2, -3)```   | 83.4      | x9.6  |
| ```fmt::format_to(charz, "nums: {}, {}, {}", -1, -2, -3)```             | 30.3      | x3.5  |

## License

ufmt is licensed under the [MIT License](https://opensource.org/licenses/MIT).
