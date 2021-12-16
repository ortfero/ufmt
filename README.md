# ufmt

C++ library to format text


## Usage

Drop `include/ufmt` at your include path


## Supported platforms and compilers

gcc 11, clang 14, msvc 19.24


## Tests

To build tests:

```shell
cd ufmt
mkdir build
cd build
meson ../test
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


## Benchmarks

### Formatting

| Code                                                         | Time (ns) | Ratio |
|--------------------------------------------------------------|----------:|------:|
| ```text.format('a');```                                     | 0.5       | x1    |
| ```snprintf(charz, sizeof (charz), "%c", 'a');```            | 25.9      | x51   |
| ```fmt::format(charz, "{}", 'a');```                      | 8.4       | x16   |
| ```texter::format("some literal");```                          | 0.5       | x1    |
| ```snprintf(charz, sizeof (charz), "%s", "some literal");``` | 51.0      | x102  |
| ```fmt::format(charz, "{}", "some literal");```           | 14.4      | x28   |
| ```texter::format(-127562);```                                 | 1.9       | x1    |
| ```snprintf(charz, sizeof (charz), "%d", -127562);```        | 44.2      | x23   |
| ```fmt::format_to(charz, "{}", -127562);```                  | 14.5      | x7    |
| ```std::to_chars(charz, charz + sizeof charz, -127562);```   | 0.8       | x0.4  |
| ```texter::format(-127562.127562);```                          | 1.9       | x1    |
| ```snprintf(charz, sizeof (charz), "%f", -127562.127562);``` | 433.5     | x228  |
| ```fmt::format(charz, "{}", -127562.127562);```           | 57.0      | x30   |

```
gcc version 10.3.0, fmtlib 8.0.1
```

## License

ufmt licensed under [MIT license](https://opensource.org/licenses/MIT).
