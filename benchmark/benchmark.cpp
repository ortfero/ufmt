#define FMT_HEADER_ONLY

#include <charconv>
#include <cstdio>
#include <format>
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
        ubench::dont_optimize(c);
        text.format(c);
        ubench::dont_optimize(text.data());
    });
    auto const snprintf_char = ubench::run([&] {
        auto c = 'x';
        ubench::dont_optimize(c);
        snprintf(charz, sizeof(charz), "%c", c);
        ubench::dont_optimize(charz);
    });
    auto const fmt_char = ubench::run([&] {
        auto c = 'x';
        ubench::dont_optimize(c);
        fmt::format_to(charz, "{}", c);
        ubench::dont_optimize(charz);
    });

    cout << "texter.print(char)    - " << texter_char << '\n';
    cout << "snprintf(char)        - " << snprintf_char << '\n';
    cout << "fmt::format_to(char)  - " << fmt_char << '\n';
    cout << endl;

    auto const texter_literal = ubench::run([&] {
        text.clear();
        text.format("some literal");
        ubench::dont_optimize(text.data());
    });
    auto const snprintf_literal = ubench::run([&] {
        snprintf(charz, sizeof(charz), "%s", "some literal");
        ubench::dont_optimize(charz);
    });
    auto const fmt_literal = ubench::run([&] {
        fmt::format_to(charz, "{}", "some literal");
        ubench::dont_optimize(charz);
    });

    cout << "texter.print(<char[N]>)    - " << texter_literal << '\n';
    cout << "snprintf(<char[N]>)        - " << snprintf_literal << '\n';
    cout << "fmt::format_to(<char[N]>)  - " << fmt_literal << '\n';
    cout << endl;

    auto const texter_int = ubench::run([&] {
        text.clear();
        text.format(rand());
        ubench::dont_optimize(text.data());
    });
    auto const snprintf_int = ubench::run([&] {
        snprintf(charz, sizeof(charz), "%d", rand());
        ubench::dont_optimize(charz);
    });
    auto const fmt_int = ubench::run([&] {
        fmt::format_to(charz, "{}", rand());
        ubench::dont_optimize(charz);
    });
    auto const tochars_int = ubench::run([&] {
        std::to_chars(charz, charz + sizeof(charz), rand());
        ubench::dont_optimize(charz);
    });
    auto const format_int = ubench::run([&] {
       std::format_to(charz, "{}", rand());
       ubench::dont_optimize(charz);
    });

    cout << "texter.print(<int>)     - " << texter_int << '\n';
    cout << "snprintf(<int>)         - " << snprintf_int << '\n';
    cout << "fmt::format_to(<int>)   - " << fmt_int << '\n';
    cout << "std::to_chars(<int>)    - " << tochars_int << '\n';
    cout << "std::format_to(<int>)   - " << format_int << '\n';
    cout << endl;

    auto const texter_double = ubench::run([&] {
        text.clear();
        text.format(rand() % 2 ? 123.123 : -123.123);
        ubench::dont_optimize(text.data());
    });
    auto const snprintf_double = ubench::run([&] {
        std::snprintf(charz, sizeof(charz), "%f", rand() % 2 ? 123.123 : -123.123);
        ubench::dont_optimize(charz);
    });
    auto const fmt_double = ubench::run([&] {
        fmt::format_to(charz, "{}", rand() % 2 ? 123.123 : -123.123);
        ubench::dont_optimize(charz);
    });
    auto const to_chars_double = ubench::run([&] {
        std::to_chars(charz, charz + sizeof(charz), rand() % 2 ? 123.123 : -123.123);
        ubench::dont_optimize(charz);
    });
    auto const format_double = ubench::run([&] {
       std::format_to(charz, "{}", rand() % 2 ? 123.123 : -123.123);
       ubench::dont_optimize(charz);
    });


    cout << "texter.print(<double>)     - " << texter_double << '\n';
    cout << "snprintf(<double>)         - " << snprintf_double << '\n';
    cout << "fmt::format_to(<double>)   - " << fmt_double << '\n';
    cout << "std::to_chars(<double>)    - " << to_chars_double << '\n';
    cout << "std::format_to(<double>)   - " << format_double << '\n';
    cout << endl;

    auto const texter_format = ubench::run([&] {
        text.clear();
        if(rand() % 2)
            text.format("nums: ", 1, ", ", 123.123, ", ", std::string{"kaka"});
        else
            text.format("nums: ", -1, ", ", -123.123, ", ", std::string{"kuku"});
        ubench::dont_optimize(text.data());
    });
    auto const snprintf_format = ubench::run([&] {
        if(rand() % 2)
            std::snprintf(charz, sizeof(charz), "nums: %d, %f, %s", 1, 123.123, std::string{"kaka"}.data());
        else
            std::snprintf(charz, sizeof(charz), "nums: %d, %f, %s", -1, -123.123, std::string{"kuku"}.data());
        ubench::dont_optimize(charz);
    });
    auto const fmt_format = ubench::run([&] {
        if(rand() %2)
            fmt::format_to(charz, "nums: {}, {}, {}", 1, 123.123, std::string{"kaka"});
        else
            fmt::format_to(charz, "nums: {}, {}, {}", -1, -123.123, std::string{"kuku"});
        ubench::dont_optimize(charz);
    });

    cout << "texter.print('nums: ', <int>, ', ', <double>, ', ', <string>)    - "
         << texter_format << '\n';
    cout << "snprintf('nums: %d, %f, %s', <int>, <double>, <string>)          - "
         << snprintf_format << '\n';
    cout << "fmt::format_to('nums: {}, {}, {}', <int>, <double>, <string>)    - "
         << fmt_format << '\n';
    cout << endl;

    return 0;
}
