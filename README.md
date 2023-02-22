# ufmt

C++17 header-only library to format text


## Usage

Drop `include/ufmt` at your include path


## Tests and benchmark

To build tests and benchmark:

```shell
cd ufmt
mkdir build
cd build
meson ..
ninja
```

## Interface

```cpp
template<typename S>
class basic_text {
public:
    
    using size_type = std::size_t;
    using value_type = typename S::value_type;
    
    template<typename... Args>
    static basic_text of(Args&&... args);    
        
    S const& string() const noexcept;
    value_type const* data() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;
    size_type capacity() const noexcept;
    value_type operator[](size_type i) const noexcept;
    value_type& operator[](size_type i) noexcept;
    void reserve(size_type n);
    value_type* allocate(size_type n);
    void free(value_type* p);
    basic_text& append(value_type const* stringz, size_type n);
    void char_n(value_type ch, size_type n);
    
    template<typename... Args>
    void format(Args&&... args);
};

using text = basic_text<std::string>;
```


## Snippets


### Print to output

```cpp
#include <ufmt/print.hpp>
...
ufmt::print("value: ", 127.562);
```


### Print to error and return value
```cpp
int foo() {
    return ufmt::error_with(-1, "Error: ", 12);
}

```


### Basic text formatting

```cpp
#include <ufmt/text.hpp>

ufmt::text r = ufmt::text::of("value: ", -12.12);
```


### Stream style

```cpp
ufmt::text text;
text << "value: " << -12.12;
```


### Floating point number with precision

```cpp
using ufmt::fixed;
auto r = ufmt::text::of(fixed(3.14159, 2));
// "3.14"
```


### Integer number of fixed width

```cpp
using ufmt::fixed;
auto r = ufmt::text::of(fixed(1, 4));
// "0001"
```


### Left alignment

```cpp
using ufmt::left;
auto r = ufmt::text::of(left(1, 4));
// "1   "
```


### Right alignment

```cpp
using ufmt::right;
auto r = ufmt::text::of(right(1, 4));
// "   1"
```


### Quoted string

```cpp
using ufmt::quoted;
auto r = ufmt::text::of(quoted("qwerty"));
// "'qwerty'"
```


### Double quoted string

```cpp
using ufmt::dquoted;
auto r = ufmt::text::of(dquoted("qwerty"));
// ""qwerty""
```


### Custom type

```cpp

struct point { double x, y; };

template<typename S> S& operator << (S& stream, point const& p) {
    return stream << "{ x: " << p.x << "; y: " << p.y << " }";
}

```


## JSON formatting

### Format object

```cpp
#include <ufmt/json.hpp>

...

std::string const& text = ufmt::json::of("x", -1, "y", 3.14, "z", "ok");
// text == R"({"x":-1,"y":3.14,"z":"ok"})"
```


### Format custom type

```cpp
#include <ufmt/json.hpp>

struct point {
    int x, y;
};

template<class S> ufmt::basic_json<S>& operator << (ufmt::basic_json<S>& json, point const& p) {
    return json << ufmt::object("x", p.x, "y", p.y);
}

...

std::string const& text = ufmt::json::of("point", point{-1, -1});
// text == R"({"point":{"x":-1,"y":-1}})"
```

### Format optional fields

```cpp
#include <optional>
#include <ufmt/json.hpp>

struct point {
    int x, y;
    std::optional<int> z;
};

template<class S> ufmt::basic_json<S>& operator << (ufmt::basic_json<S>& json, point const& p) {
    return json << ufmt::object("x", p.x, "y", p.y, "z", p.z);
}

...

std::string const& text1 = ufmt::json::of("point", point{-1, -1, -1});
// text1 == R"({"point":{"x":-1,"y":-1,"z":-1}})"
std::string const& text2 = ufmt::json::of("point", point{-1, -1, std::nullopt});
// text1 == R"({"point":{"x":-1,"y":-1}})"
```

### Format array

```cpp
auto const& text = ufmt::json::of("x", std::vector{1, 2, 3}); // std::array should be ok too
// text == R"({"x":[1,2,3]})";

```

### Format to fixed buffer

```cpp
auto const& text = ufmt::long_string_json::of("x", -1);
// long_string_json - around 1024 bytes
// page_string_json - around 4096 bytes 
// double_page_string_json - around 8192 bytes
// large_string_json - around 65536 bytes
```


### Format to reserved buffer

```cpp
auto json = ufmt::json{};
json.reserve(65536);
json << ufmt::object("x", -1, "y", 3.14, "z", "ok");
auto const& text = json.string();
```


## Benchmarks

### Formatting

| Code                                                         | Time (ns) | Ratio |
|--------------------------------------------------------------|----------:|------:|
| ```text.format('a');```                                      | 7.7       | x1    |
| ```snprintf(charz, sizeof (charz), "%c", 'a');```            | 38.4      | x5    |
| ```fmt::format_to(charz, "{}", 'a');```                      | 13.8      | x1.8  |
| ```text.format("some literal");```                           | 11.1      | x1    |
| ```snprintf(charz, sizeof (charz), "%s", "some literal");``` | 38.4      | x3.4  |
| ```fmt::format_to(charz, "{}", "some literal");```           | 13.6      | x1.2  |
| ```text.format(-127562);```                                  | 13.9      | x1    |
| ```snprintf(charz, sizeof (charz), "%d", -127562);```        | 58.9      | x4.2  |
| ```fmt::format_to(charz, "{}", -127562);```                  | 15.9      | x1.1  |
| ```std::to_chars(charz, charz + sizeof charz, -127562);```   | l8.2      | x0.6  |
| ```text.format(-127562.127562);```                           | 45.3      | x1    |
| ```snprintf(charz, sizeof (charz), "%f", -127562.127562);``` | 237.0     | x5.2  |
| ```fmt::format_to(charz, "{}", -127562.127562);```           | 88.5      | x1.9  |

```
msvc version 19.29, fmtlib 8.0.1
```

## License

ufmt licensed under [MIT license](https://opensource.org/licenses/MIT).
