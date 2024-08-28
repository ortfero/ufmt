#define FMT_HEADER_ONLY

#include <charconv>
#include <cstdio>
#include <iostream>

#include <fmt/core.h>
#include <ufmt/text.hpp>

#include "ubench.hpp"


int main() {
    using namespace std;

    ufmt::fixed_text text;
    char charz[128];

    auto const texter_char = ubench::run([&] {
        text.clear();
        auto c = 'x';
        text.format(c);
    });
    auto const snprintf_char = ubench::run([&] {
        auto c = 'x';
        snprintf(charz, sizeof(charz), "%c", c);
    });
    auto const fmt_char = ubench::run([&] {
        auto c = 'x';
        fmt::format_to(charz, "{}", c);
    });

    cout << "texter.print(char)    - " << texter_char << '\n';
    cout << "snprintf(char)        - " << snprintf_char << '\n';
    cout << "fmt::format_to(char)  - " << fmt_char << '\n';
    cout << endl;

    auto const texter_literal = ubench::run([&] {
        text.clear();
        text.format("some literal");
    });
    auto const snprintf_literal = ubench::run([&] {
        snprintf(charz, sizeof(charz), "%s", "some literal");
    });
    auto const fmt_literal = ubench::run([&] {
        fmt::format_to(charz, "{}", "some literal");
    });

    cout << "texter.print(char[N])    - " << texter_literal << '\n';
    cout << "snprintf(char[N])        - " << snprintf_literal << '\n';
    cout << "fmt::format_to(char[N])  - " << fmt_literal << '\n';
    cout << endl;

    auto const texter_int = ubench::run([&] {
        text.clear();
        text.format(rand());
    });
    auto const snprintf_int = ubench::run([&] {
        snprintf(charz, sizeof(charz), "%d", rand());
    });
    auto const fmt_int = ubench::run([&] {
        fmt::format_to(charz, "{}", rand());
    });
    auto const tochars_int = ubench::run([&] {
        std::to_chars(charz, charz + sizeof(charz), rand());
    });

    cout << "texter.print(int)     - " << texter_int << '\n';
    cout << "snprintf(int)         - " << snprintf_int << '\n';
    cout << "fmt::format_to(int)   - " << fmt_int << '\n';
    cout << "std::to_chars(int)    - " << tochars_int << '\n';
    cout << endl;

    auto const texter_double = ubench::run([&] {
        text.clear();
        text.format(-127562.127562);
    });
    auto const snprintf_double = ubench::run(
        [&] { snprintf(charz, sizeof(charz), "%f", -127562.127562); });
    auto const fmt_double =
        ubench::run([&] { fmt::format_to(charz, "{}", -127562.127562); });


    cout << "texter.print(double)     - " << texter_double << '\n';
    cout << "snprintf(double)         - " << snprintf_double << '\n';
    cout << "fmt::format_to(double)   - " << fmt_double << '\n';
    cout << endl;

    auto const texter_format = ubench::run([&] {
        text.clear();
        text.format("nums: ", -1, ", ", -2, ", ", -3);
    });
    auto const snprintf_format = ubench::run([&] {
        snprintf(charz, sizeof(charz), "nums: %d, %d, %d", -1, -2, -3);
    });
    auto const fmt_format = ubench::run(
        [&] { fmt::format_to(charz, "nums: {}, {}, {}", -1, -2, -3); });

    cout << "texter.print('nums: ', -1, ', ', -2, ', ', -3)    - "
         << texter_format << '\n';
    cout << "snprintf('nums: %d, %d, %d', -1, -2, -3)          - "
         << snprintf_format << '\n';
    cout << "fmt::format_to('nums: {}, {}, {}', -1, -2, -3)    - "
         << fmt_format << '\n';
    cout << endl;

    return 0;
}
