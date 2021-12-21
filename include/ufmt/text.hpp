/* This file is part of ufmt library
 * Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <charconv>
#include <string>
#include <string_view>
#include <vector>


namespace ufmt {

    
    template<typename S>
    class basic_text {
    private:
        
        S string_;
        
    public:
        
        using size_type = std::size_t;
        using value_type = typename S::value_type;
        
        
        template<typename... Args>
        static basic_text of(Args&&... args) {
            basic_text t;
            t.format(std::forward<Args>(args)...);
            return t;
        }
        
        
        basic_text() noexcept = default;
        basic_text(basic_text const&) = default;
        basic_text& operator=(basic_text const&) = default;
        basic_text(basic_text&&) noexcept = default;
        basic_text& operator=(basic_text&&) noexcept = default;
        
        S const& string() const noexcept { return string_; }
        value_type const* data() const noexcept { return string_.data(); }
        size_type size() const noexcept { return string_.size(); }
        bool empty() const noexcept { return string_.empty(); }
        void clear() noexcept { string_.clear(); }
        size_type capacity() const noexcept { return string_.capacity(); }
        value_type operator[](size_type i) const noexcept { return string_[i]; }
        value_type& operator[](size_type i) noexcept { return string_[i]; }
        
        
        void reserve(size_type n) {
            if(n <= string_.capacity())
                return;
            n = nearest_power_of_2(n);
            string_.reserve(n);
        }
        
        
        value_type* allocate(size_type n) {
            auto const original_size = string_.size();
            auto const next_size = original_size + n;
            auto const original_capacity = string_.capacity();
            if(next_size > string_.capacity()) {
                auto const reserve_size = nearest_power_of_2(next_size);
                string_.reserve(reserve_size);
                auto const next_capacity = string_.capacity();
                if(string_.capacity() < reserve_size)
                    return nullptr;
            }
            string_.resize(next_size);
            return string_.data() + original_size;
        }
        
        
        void free(value_type* p) {
            string_.resize(size_type(p - string_.data()));
        }
        
        
        basic_text& append(value_type const* stringz, size_type n) {
            if(n == 0)
                return *this;
            value_type* p = allocate(n);
            if(!p)
                return *this;
            value_type const* end = stringz + n;
            for(value_type const* from = stringz; from != end; ++from, ++p)
                *p = *from;
            free(p);
            return *this;
        }
        
        
        void char_n(value_type ch, size_type n) {
            value_type* p = allocate(n);
            if(!p)
                return;
            for(value_type* end = p + n; p != end; ++p)
                *p = ch;
            free(p);
        }
        
        
        void format() {
        }
        
        
        template<typename Arg, typename... Args>
        void format(Arg&& arg, Args&&... args) {
            (*this) << arg;
            format(std::forward<Args>(args)...);
        }


        template<std::size_t N> friend
        basic_text& operator << (basic_text& self,
                                 value_type const (&literal)[N]) {
            return self.append(literal, N - 1);
        }
        
        
    private:
        
        static uint64_t nearest_power_of_2(uint64_t n) {
            if(n <= 2)
                return 2;
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n |= n >> 32;
            n++;
            return n;
        }
    }; // basic_text
    
    
    using text = basic_text<std::string>;
    
    
    template<class String, class Stream>
    Stream& operator << (Stream& stream,
                         basic_text<String> const& bt) {
        return stream << bt.string();
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self,
                                typename S::value_type ch) {
        typename S::value_type* p = self.allocate(1);
        if(!p)
            return self;
        *p++ = ch;
        self.free(p);
        return self;
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::string_view sv) {
        return self.append(sv.data(), sv.size());
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::string const& s) {
        return self.append(s.data(), s.size());
    }
    
    
    /*template<class S>
    basic_text<S>& operator << (basic_text<S>& self, bool value) {
        return value ? self.append("true", 4) : self.append("false", 5);
    }*/
    
    
    namespace detail {
        
        template<std::size_t N, class S, typename T>
        basic_text<S>& print_number(basic_text<S>& self, T value) {
            typename S::value_type* p = self.allocate(N);
            if(!p)
                return self;
            auto const r = std::to_chars(p, p + N, value);
            self.free(r.ptr);
            return self;
        }
        
    } // detail
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::int32_t value) {
        return detail::print_number<10>(self, value);
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::uint32_t value) {
        return detail::print_number<11>(self, value);
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::int64_t value) {
        return detail::print_number<19>(self, value);
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::uint64_t value) {
        return detail::print_number<18>(self, value);
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, float value) {
        return detail::print_number<64>(self, value);
    }
    
    
    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, double value) {
        return detail::print_number<64>(self, value);
    }
    
    
    template<class S, typename T>
    basic_text<S>& operator << (basic_text<S>& self,
                                std::vector<T> const& value) {
        self << '[';
        if(!value.empty()) {
            auto it = value.begin();
            self << ' ' << *it;
            ++it;
            for(; it != value.end(); ++it)
                self << ", " << *it;
        }
        self << " ]";
        return self;
    }
    
    
    namespace formatters {
        
        template<typename T>
        struct left {
            T const& value;
            std::size_t width;
        };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, left<T> l) {
            auto const original_size = self.size();
            self << l.value;
            auto const next_size = self.size();
            auto const value_size = next_size - original_size;
            if(value_size >= l.width)
                return self;
            auto const spaces_count = l.width - value_size;
            self.char_n(' ', spaces_count);
            return self;
        }
        
        
        template<typename T>
        struct right {
            T const& value;
            std::size_t width;
        };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, right<T> r) {
            auto const original_size = self.size();
            self << r.value;
            auto const next_size = self.size();
            auto const value_size = next_size - original_size;
            if(value_size >= r.width)
                return self;
            auto const spaces_count = r.width - value_size;
            self.char_n(' ', spaces_count);
            for(auto i = next_size - 1; i != original_size - 1; --i)
                self[i + spaces_count] = self[i];
            for(std::size_t i = 0; i != spaces_count; ++i)
                self[original_size + i] = ' ';
            return self;
        }
        
        
        template<typename T>
        struct float_fixed {
            T value;
            int precision;
        };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, float_fixed<T> f) {
            constexpr auto float_digits = 64;
            typename S::value_type* p = self.allocate(float_digits);
            if(!p)
                return self;
            auto const r = std::to_chars(p, p + float_digits, f.value, 
                                         std::chars_format::fixed, f.precision);
            self.free(r.ptr);
            return self;
        }
        
        
        template<typename T>
        struct integer_fixed {
            T value;
            std::size_t width;
        };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, integer_fixed<T> f) {
            auto const original_size = self.size();
            self << f.value;
            auto const next_size = self.size();
            auto const value_size = next_size - original_size;
            if(value_size >= f.width)
                return self;
            auto const spaces_count = f.width - value_size;
            self.char_n('0', spaces_count);
            for(auto i = next_size - 1; i != original_size - 1; --i)
                self[i + spaces_count] = self[i];
            for(std::size_t i = 0; i != spaces_count; ++i)
                self[original_size + i] = '0';
            return self;
        }
        
        
        template<typename T>
        struct quoted { T const& value; };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, quoted<T> q) {
            self << '\'';
            self << q.value;
            self << '\'';
            return self;
        }
        
        
        template<typename T>
        struct dquoted { T const& value; };
        
        
        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, dquoted<T> q) {
            self << '\"';
            self << q.value;
            self << '\"';
            return self;
        }
    } // formatters
    
    
    template<typename T>
    formatters::left<T> left(T const& value, std::size_t width) {
        return formatters::left<T>{value, width};
    }
    
    
    template<typename T>
    formatters::right<T> right(T const& value, std::size_t width) {
        return formatters::right<T>{value, width};
    }
    
    
    formatters::float_fixed<double> precised(double value, int precision) {
        return formatters::float_fixed<double>{value, precision};
    }
    
    
    formatters::integer_fixed<std::uint64_t> fixed(std::uint64_t value, int width) {
        return formatters::integer_fixed<std::uint64_t>{value, std::size_t(width)};
    }
    
    
    template<typename T>
    formatters::quoted<T> quoted(T const& value) {
        return formatters::quoted<T>{value};
    }
    
    
    template<typename T>
    formatters::dquoted<T> dquoted(T const& value) {
        return formatters::dquoted<T>{value};
    }
    
} // ufmt
