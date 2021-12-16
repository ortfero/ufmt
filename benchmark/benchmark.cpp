#include <iostream>
#include <cstdio>
#include <charconv>

#include <fmt/core.h>
#include <ufmt/text.hpp>

#include "ubench.hpp"


int main() {

  using namespace std;

  ufmt::text text;
  char charz[128];

  auto const texter_char = ubench::run([&]{ text.clear(); text.format('a'); });
  auto const snprintf_char = ubench::run([&]{ snprintf(charz, sizeof (charz), "%c", 'a'); });
  auto const fmt_char = ubench::run([&]{ fmt::format_to(charz, "{}", 'a'); });

  cout << "texter.print(char) - " << texter_char << '\n';
  cout << "snprintf(char)     - " << snprintf_char << '\n';
  cout << "fmt::format(char)  - " << fmt_char << '\n';
  cout << endl;

  auto const texter_literal = ubench::run([&]{ text.clear(); text.format("some literal"); });
  auto const snprintf_literal = ubench::run([&]{ snprintf(charz, sizeof (charz), "%s", "some literal"); });
  auto const fmt_literal = ubench::run([&]{ fmt::format_to(charz, "{}", "some literal"); });

  cout << "texter.print(char[N]) - " << texter_literal << '\n';
  cout << "snprintf(char[N])     - " << snprintf_literal << '\n';
  cout << "fmt::format(char[N])  - " << fmt_literal << '\n';
  cout << endl;

  auto const texter_int = ubench::run([&]{ text.clear(); text.format(-127562); });
  auto const snprintf_int = ubench::run([&]{ snprintf(charz, sizeof (charz), "%d", -127562); });
  auto const fmt_int = ubench::run([&]{ fmt::format_to(charz,  "{}", -127562); });
  auto const tochars_int = ubench::run([&] { std::to_chars(charz, charz + sizeof(charz), -127562); });

  cout << "texter.print(int)  - " << texter_int << '\n';
  cout << "snprintf(int)      - " << snprintf_int << '\n';
  cout << "fmt::format(int)   - " << fmt_int << '\n';
  cout << "std::to_chars(int) - " << tochars_int << '\n';
  cout << endl;

  auto const texter_double = ubench::run([&]{ text.clear(); text.format(-127562.127562); });
  auto const snprintf_double = ubench::run([&]{ snprintf(charz, sizeof (charz), "%f", -127562.127562); });
  auto const fmt_double = ubench::run([&]{ fmt::format_to(charz,  "{}", -127562.127562); });


  cout << "texter.print(double)  - " << texter_double << '\n';
  cout << "snprintf(double)      - " << snprintf_double << '\n';
  cout << "fmt::format(double)   - " << fmt_double << '\n';
  cout << endl;


  return 0;
}
