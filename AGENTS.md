# Repository Guidelines

## Project Structure & Module Organization
- `include/ufmt/*.hpp`: Public header-only API (text formatting, JSON helpers, printing, fixed buffers). Keep exports stable and minimal.
- `test/`: Doctest suites (`*.test.hpp`) aggregated by `test/test.cpp`. Add new suites here and wire them into the main.
- `benchmark/`: Microbenchmarks using `ubench.hpp`; produces the `ufmt-bench` binary.
- `thirdparty/`: Vendored dependencies (e.g., fmt headers). Avoid modifying unless bumping versions.
- `build/`: Generated binaries (`ufmt-test`, `ufmt-bench`); created by `just` recipes.

## Build, Test, and Development Commands
- `just test`: Build and run the doctest suite (debug flags, C++20).
- `just bench`: Build and execute benchmarks with release flags (`-O3 -DNDEBUG`).
- `just build`: Build both test and benchmark binaries without running them.
- `just clean`: Remove `build/`. Re-run `just test` after cleanup to restore artifacts.
- If `just` is unavailable, mirror `justfile` commands manually (e.g., `c++ test/test.cpp -o build/ufmt-test -std=c++20 -Iinclude -Ithirdparty/include`).

## Coding Style & Naming Conventions
- C++20, header-only; prefer standard library utilities and avoid adding new dependencies.
- 4-space indentation, braces on the same line as declarations, and tight spacing around control flow (see existing headers for patterns).
- Use `snake_case` for functions/variables, `PascalCase` for types; keep public API names concise and lowercase (`ufmt::text`, `ufmt::json`).
- Mark lightweight accessors `noexcept` and favor `std::string_view`/`span`-like views over copies.
- Keep includes ordered and minimal; always include `<ufmt/...>` via the unified headers under `include/ufmt`.

## Testing Guidelines
- Framework: [doctest](https://github.com/doctest/doctest) bundled in `test/doctest.h`.
- Add new cases in a relevant `*.test.hpp` under a `TEST_SUITE`/`SCENARIO` block; include the file from `test/test.cpp`.
- Run `just test` before pushing; keep tests deterministic and fast enough for CI-friendly runs.
- When adding APIs, cover both happy paths and boundary conditions (buffer limits, empty data, precision/width handling).

## Commit & Pull Request Guidelines
- Commits: short, imperative subject lines (e.g., `Add width handling for fixed numbers`); group related changes per commit.
- PRs: describe the change, rationale, and user-facing impact; link issues when present. Note any API surface changes or benchmarks that moved.
- Validation: list commands executed (`just test`, `just bench` if relevant) and attach benchmark comparisons when performance is affected.
- Keep diffs minimal; avoid reformatting unrelated areas to ease review of header-only code.
