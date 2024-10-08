// This file is part of ufmt library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <charconv>
#include <cstdint>
#include <cmath>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#if defined(_MSC_VER)
#include <cstdio>
#endif

#include <ufmt/fixed_string.hpp>


namespace ufmt {


    template<typename S>
    class basic_text {
    private:

        S string_;

    public:

        using size_type = std::size_t;
        using value_type = typename S::value_type;


        template<typename... Args>
        static S of(Args&&... args) {
            basic_text t;
            t.format(std::forward<Args>(args)...);
            return t.string_;
        }


        basic_text() noexcept = default;
        basic_text(basic_text const&) = default;
        basic_text& operator=(basic_text const&) = default;
        basic_text(basic_text&&) noexcept = default;
        basic_text& operator=(basic_text&&) noexcept = default;

        S const& string() const & noexcept { return string_; }
        S&& string() && noexcept { return std::move(string_); }
        value_type const* data() const noexcept { return string_.data(); }
        size_type size() const noexcept { return string_.size(); }
        bool empty() const noexcept { return string_.empty(); }
        void clear() noexcept { string_.clear(); }
        size_type capacity() const noexcept { return string_.capacity(); }        
        value_type operator[](size_type i) const noexcept { return string_[i]; }
        value_type& operator[](size_type i) noexcept { return string_[i]; }
        
        
        std::string_view view() const noexcept {
            return std::string_view{string_.data(), string_.size()};
        }


        void reserve(size_type n) {
            if(n <= string_.capacity())
                return;
            n = nearest_power_of_2(n);
            string_.reserve(n);
        }


        value_type* allocate(size_type n) {
            auto const original_size = string_.size();
            auto const next_size = original_size + n;
            if(next_size > string_.capacity()) {
                auto const reserve_size = nearest_power_of_2(next_size);
                string_.reserve(reserve_size);
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
            if(n == 0)
                return;
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
    using short_text = basic_text<short_string>;
    using fixed_text = basic_text<string>;


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


    template<class S, std::size_t N>
    basic_text<S>& operator << (basic_text<S>& self, char const (&cc)[N]) {
        return self.append(cc, N - 1);
    }


    namespace detail {

        template<std::size_t N, class S, typename T>
        basic_text<S>& print_number(basic_text<S>& self, T value) {
            typename S::value_type* p = self.allocate(N);
            if(!p)
                return self;
            auto const r = std::to_chars(p, p + N, value);
			if(r.ec == std::errc{}) {
				self.free(r.ptr);
			} else {
				self.free(p);
			}

            return self;
        }


        template<std::size_t N, class S>
        basic_text<S>& print_number(basic_text<S>& self, double value) {
            typename S::value_type* p = self.allocate(N);
            if(!p)
                return self;
            auto const r = std::to_chars(p, p + N, value);
			if(r.ec == std::errc{}) {
				self.free(r.ptr);
			} else {
				self.free(p);
			}
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
        return detail::print_number<20>(self, value);
    }


    template<class S>
    basic_text<S>& operator << (basic_text<S>& self, std::uint64_t value) {
        return detail::print_number<20>(self, value);
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
                self << ',' << ' ' << *it;
        }
        self << ' ' << ']';
        return self;
    }


    template<class S, typename T>
    basic_text<S>& operator << (basic_text<S>& self,
                                std::span<T> const& value) {
        self << '[';
        if(!value.empty()) {
            auto it = value.begin();
            self << ' ' << *it;
            ++it;
            for(; it != value.end(); ++it)
                self << ',' << ' ' << *it;
        }
        self << ' ' << ']';
        return self;
    }


    namespace formatters {

        template<typename T>
        struct left {
            T const& value;
            unsigned width;
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
            unsigned width;
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
        struct precised {
            T value;
            int precision;
        };


        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, precised<T> f) {
            constexpr auto float_digits = 64;
            typename S::value_type* p = self.allocate(float_digits);
            if(!p)
                return self;
        #if defined(_MSC_VER)
            auto const r = std::to_chars(p, p + float_digits, f.value,
                                         std::chars_format::fixed, f.precision);
            if(r.ec == std::errc{}) {
				self.free(r.ptr);
			} else {
				self.free(p);
			}
        #else
            auto const n = std::snprintf(p, float_digits, "%.*f", f.precision, f.value);
            if(n <= 0) self.free(p); else self.free(p + n);
        #endif
            return self;
        }


        template<typename T>
        struct fixed {
            T value;
            unsigned width;
        };


        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, fixed<T> f) {
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
            return self << '\'' << q.value << '\'';
        }


        template<typename T>
        struct dquoted { T const& value; };


        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, dquoted<T> q) {
            return self << '\"' << q.value << '\"';
        }

        template<typename T>
        struct textize { T const& value; };

        template<class S>
        basic_text<S>& operator << (basic_text<S>& self, textize<char> arg) {
            return self << '\"' << arg.value << '\"';
        }

        template<class S>
        basic_text<S>& operator << (basic_text<S>& self, textize<std::string_view> arg) {
            return self << '\"' << arg.value << '\"';
        }

        template<class S>
        basic_text<S>& operator << (basic_text<S>& self, textize<std::string> arg) {
            return self << '\"' << arg.value << '\"';
        }
        
        
        template<class S, std::size_t N>
        basic_text<S>& operator << (basic_text<S>& self, textize<fixed_string<N>> arg) {
            return self << '\"' << arg.value << '\"';
        }
        
        
        template<class S, class T>
        basic_text<S>& operator << (basic_text<S>& self, textize<basic_text<T>> arg) {
            return self << '\"' << arg.value << '\"';
        }
        

        template<class S, typename T>
        basic_text<S>& operator << (basic_text<S>& self, textize<T> arg) {
            return self << arg.value;
        }
        
        
        struct boolean {
            bool value;
        };
        
        
        template<class S> S& operator << (S& self, boolean arg) {
            if(arg.value)
                return self << "true";
            else
                return self << "false";
        }
        
        
        struct char_n {
            char c;
            std::size_t n;
        }; // char_n
        
        
        template<class S>
        basic_text<S>& operator << (basic_text<S>& self, char_n arg) {
            self.char_n(arg.c, arg.n);
            return self;
        }
			
    } // formatters


    template<typename T>
    formatters::left<T> left(T const& value, unsigned width) noexcept {
        return formatters::left<T>{value, width};
    }


    template<typename T>
    formatters::right<T> right(T const& value, unsigned width) noexcept {
        return formatters::right<T>{value, width};
    }


    inline formatters::precised<double> precised(double value, int precision) noexcept {
        return formatters::precised<double>{value, precision};
    }

    inline formatters::fixed<std::int32_t> fixed(std::int32_t value, unsigned width) noexcept {
        return formatters::fixed<std::int32_t>{value, unsigned(width)};
    }

    inline formatters::fixed<std::uint32_t> fixed(std::uint32_t value, unsigned width) noexcept {
        return formatters::fixed<std::uint32_t>{value, unsigned(width)};
    }

    inline formatters::fixed<std::int64_t> fixed(std::int64_t value, unsigned width) noexcept {
        return formatters::fixed<std::int64_t>{value, unsigned(width)};
    }

    inline formatters::fixed<std::uint64_t> fixed(std::uint64_t value, unsigned width) noexcept {
        return formatters::fixed<std::uint64_t>{value, unsigned(width)};
    }


    template<typename T>
    formatters::quoted<T> quoted(T const& value) noexcept {
        return formatters::quoted<T>{value};
    }


    template<typename T>
    formatters::dquoted<T> dquoted(T const& value) noexcept {
        return formatters::dquoted<T>{value};
    }


    template<typename T>
    formatters::textize<T> textize(T const& value) noexcept {
        return formatters::textize<T>{value};
    }
    
    
    inline formatters::boolean boolean(bool value) noexcept {
        return formatters::boolean{value};
    }
    
    
    inline formatters::char_n char_n(char c, std::size_t n) {
        return formatters::char_n{c, n};
    }
	
} // ufmt
