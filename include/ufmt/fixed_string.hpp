// This file is part of ufmt library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <cstddef>
#include <stdexcept>
#include <string_view>


namespace ufmt {


    template<std::size_t N>
    class fixed_string {
    public:
        using value_type = char;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = char*;
        using const_pointer = char const*;
        using reference = char&;
        using const_reference = char const&;
        using iterator = char*;
        using const_iterator = char const*;

        static constexpr size_type npos = size_type(-1);

        fixed_string() noexcept: n_(0) { p_[0] = '\0'; }

        fixed_string(fixed_string const& rhs) noexcept {
            assign(rhs.begin(), rhs.end());
        }

        template<std::size_t M>
        fixed_string(fixed_string<M> const& rhs) noexcept {
            assign(rhs.begin(), rhs.end());
        }

        template<std::size_t M>
        fixed_string(char const (&data)[M]) noexcept {
            assign(data, data + M);
        }

        template<std::size_t M>
        explicit fixed_string(wchar_t const (&data)[M]) noexcept {
            assign(data, data + M);
        }

        explicit fixed_string(char const* data) noexcept { assign(data); }

        explicit fixed_string(wchar_t const* data) noexcept { assign(data); }

        fixed_string(char const* b, char const* e) noexcept { assign(b, e); }

        fixed_string(wchar_t const* b, wchar_t const* e) noexcept {
            assign(b, e);
        }

        explicit fixed_string(std::string const& rhs) noexcept {
            assign(rhs.data(), rhs.data() + rhs.size());
        }

        explicit fixed_string(std::string_view const& rhs) noexcept {
            assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& operator=(fixed_string const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        template<std::size_t M>
        fixed_string& operator=(fixed_string<M> const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& operator=(char const* data) noexcept {
            return assign(data);
        }

        fixed_string& operator=(std::string const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& operator=(std::string_view const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        char* begin() noexcept { return p_; }
        char* end() noexcept { return p_ + n_; }
        char const* begin() const noexcept { return p_; }
        char const* end() const noexcept { return p_ + n_; }
        char const* cbegin() const noexcept { return p_; }
        char const* cend() const noexcept { return p_ + n_; }

        size_type size() const noexcept { return n_; }
        size_type length() const noexcept { return n_; }
        size_type max_size() const noexcept { return N; }
        constexpr size_type capacity() const noexcept { return N; }
        bool empty() const noexcept { return n_ == 0; }
        void clear() noexcept {
            n_ = 0;
            p_[0] = '\0';
        }
        void reserve(size_type) noexcept {}

        void resize(size_type n) noexcept {
            if(n > N)
                return;
            n_ = n;
            p_[n] = '\0';
        }

        char& operator[](size_type i) noexcept { return p_[i]; }

        char const& operator[](size_type i) const noexcept { return p_[i]; }

        char& at(size_type i) {
            if(i >= n_)
                throw std::out_of_range("invalid string index");
            return p_[i];
        }

        char const& at(size_type i) const {
            if(i >= n_)
                throw std::out_of_range("invalid string index");
            return p_[i];
        }

        char& back() noexcept { return p_[n_ - 1]; }
        char const& back() const noexcept { return p_[n_ - 1]; }
        char& front() noexcept { return p_[0]; }
        char const& front() const noexcept { return p_[0]; }

        fixed_string& operator+=(char c) noexcept {
            push_back(c);
            return *this;
        }

        template<std::size_t M>
        fixed_string& operator+=(fixed_string<M> const& rhs) noexcept {
            return append(rhs.begin(), rhs.end());
        }

        fixed_string& operator+=(char const* cc) noexcept { return append(cc); }

        fixed_string& operator+=(std::string const& rhs) noexcept {
            return append(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& operator+=(std::string_view const& rhs) noexcept {
            return append(rhs.data(), rhs.data() + rhs.size());
        }

        template<std::size_t M>
        fixed_string& append(fixed_string<M> const& rhs) noexcept {
            return append(rhs.begin(), rhs.end());
        }

        fixed_string& append(std::string const& rhs) noexcept {
            return append(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& append(std::string_view const& rhs) noexcept {
            return append(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& append(char const* cc, size_type n) noexcept {
            return append(cc, cc + n);
        }

        fixed_string& append(wchar_t const* cc, size_type n) noexcept {
            return append(cc, cc + n);
        }

        fixed_string& append(char const* cc) noexcept {
            size_type n = n_;
            if(n == N || !cc)
                return *this;
            char* c = p_ + n;
            while(n != N && *cc != '\0') {
                *c++ = *cc++;
                ++n;
            }
            *c = '\0';
            n_ = n;
            return *this;
        }

        fixed_string& append(wchar_t const* cc) noexcept {
            size_type n = n_;
            if(n == N || !cc)
                return *this;
            char* c = p_ + n;
            while(n != N && *cc != '\0') {
                *c++ = static_cast<char>(*cc++);
                ++n;
            }
            *c = '\0';
            n_ = n;
            return *this;
        }

        fixed_string& append(char const* b, char const* e) noexcept {
            if(!b || !e || b == e)
                return *this;
            size_type const m = static_cast<size_type>(e - b);
            size_type const l = N - n_;
            size_type const n = m > l ? l : m;
            e = b + n;
            for(char* p = p_ + n_; b != e; ++p, ++b)
                *p = *b;
            n_ += n;
            p_[n_] = '\0';
            return *this;
        }

        fixed_string& append(wchar_t const* b, wchar_t const* e) noexcept {
            if(!b || !e || b == e)
                return *this;
            size_type const m = static_cast<size_type>(e - b);
            size_type const l = N - n_;
            size_type const n = m > l ? l : m;
            e = b + n;
            for(char* p = p_ + n_; b != e; ++p, ++b)
                *p = static_cast<char>(*b);
            n_ += n;
            p_[n_] = '\0';
            return *this;
        }

        void push_back(char c) noexcept {
            if(n_ >= N)
                return;
            p_[n_++] = c;
            p_[n_] = '\0';
        }

        void pop_back() noexcept {
            if(n_ == 0)
                return;
            p_[--n_] = '\0';
        }

        fixed_string& assign(fixed_string const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        template<int M>
        fixed_string& assign(fixed_string<M> const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& assign(std::string const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& assign(std::string_view const& rhs) noexcept {
            return assign(rhs.data(), rhs.data() + rhs.size());
        }

        fixed_string& assign(char const* data) noexcept {
            n_ = 0;
            p_[0] = '\0';
            return append(data);
        }

        fixed_string& assign(wchar_t const* data) noexcept {
            n_ = 0;
            p_[0] = '\0';
            return append(data);
        }

        fixed_string& assign(char const* b, char const* e) noexcept {
            n_ = 0;
            p_[0] = '\0';
            return append(b, e);
        }

        fixed_string& assign(wchar_t const* b, wchar_t const* e) noexcept {
            n_ = 0;
            p_[0] = '\0';
            return append(b, e);
        }

        char const* c_str() const noexcept { return p_; }
        char const* data() const noexcept { return p_; }
        char* data() noexcept { return p_; }

        size_type find_first_of(char c) const noexcept {
            return find_first_of(c, 0);
        }

        size_type find_first_of(char c, size_type i) const noexcept {
            if(i >= n_)
                return npos;
            for(char const *p = p_ + i, *e = p_ + n_; p != e; ++p)
                if(*p == c)
                    return static_cast<size_type>(p - p_);
            return npos;
        }

        size_type find_last_of(char c) const noexcept {
            return find_last_of(c, n_ - 1);
        }

        size_type find_last_of(char c, size_type i) const noexcept {
            for(char const* p = p_ + i; p != p_ - 1; --p)
                if(*p == c)
                    return static_cast<size_type>(p - p_);
            return npos;
        }

        std::string_view substr(size_type pos, size_type n) const noexcept {
            return std::string_view {p_ + pos, p_ + pos + n};
        }
        
        int compare(fixed_string const& rhs) const noexcept {
            return compare(rhs.begin(), rhs.end());
        }

        template<int M>
        int compare(fixed_string<M> const& rhs) const noexcept {
            return compare(rhs.begin(), rhs.end());
        }

        int compare(std::string const& rhs) const noexcept {
            return compare(rhs.begin(), rhs.end());
        }

        int compare(std::string_view const& rhs) const noexcept {
            return compare(rhs.begin(), rhs.end());
        }

        int compare(char const* b, char const* e) const noexcept {
            if(b == e)
                return n_ == 0 ? 0 : 1;
            char const* c = p_;
            while(*c == *b && b != e) {
                ++c;
                ++b;
            }
            if(b == e)
                return 0;
            else if(*c > *b)
                return 1;
            else
                return -1;
        }

        int compare(char const* s) const noexcept {
            if(s == nullptr)
                return n_ == 0 ? 0 : 1;
            char const* c = p_;
            while(*c == *s && *c != '\0') {
                ++c;
                ++s;
            }
            if(*c == *s)
                return 0;
            else if(*c > *s)
                return 1;
            else
                return -1;
        }

    private:
        size_type n_;
        char p_[N + 1];

    };   //  fixed_string


    using short_string = fixed_string<96 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(short_string) == 96);
    
    using string = fixed_string<256 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(string) == 256);
    
    using long_string = fixed_string<1024 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(long_string) == 1024);
    
    using page_string = fixed_string<4092 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(page_string) == 4096);
    
    using double_page_string = fixed_string<8192 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(double_page_string) == 8192);
    
    using large_string = fixed_string<65536 - sizeof(std::size_t) - 1>;
    static_assert(sizeof(large_string) == 65536);


    template<std::size_t N>
    std::string& operator+=(std::string const& x, fixed_string<N> const& y) {
        return x.append(y.data(), y.size());
    }


    template<std::size_t N, std::size_t M>
    bool operator==(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) == 0;
    }


    template<std::size_t N>
    bool operator==(fixed_string<N> const& x, char const* y) {
        return x.compare(y) == 0;
    }


    template<std::size_t N>
    bool operator==(char const* x, fixed_string<N> const& y) {
        return y.compare(x) == 0;
    }


    template<std::size_t N>
    bool operator==(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) == 0;
    }


    template<std::size_t N>
    bool operator==(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) == 0;
    }


    template<std::size_t N>
    bool operator==(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) == 0;
    }


    template<std::size_t N>
    bool operator==(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) == 0;
    }


    template<std::size_t N, std::size_t M>
    bool operator!=(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator!=(fixed_string<N> const& x, char const* y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator!=(char const* x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator!=(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator!=(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator!=(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator!=(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N, std::size_t M>
    bool operator<(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<(fixed_string<N> const& x, char const* y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<(char const* x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator<(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator<(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N, std::size_t M>
    bool operator<=(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<=(fixed_string<N> const& x, char const* y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<=(char const* x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator<=(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<=(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator<=(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator<=(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N, std::size_t M>
    bool operator>(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>(fixed_string<N> const& x, char const* y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>(char const* x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator>(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator>(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N, std::size_t M>
    bool operator>=(fixed_string<N> const& x, fixed_string<M> const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>=(fixed_string<N> const& x, char const* y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>=(char const* x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator>=(fixed_string<N> const& x, std::string const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>=(std::string const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<std::size_t N>
    bool operator>=(fixed_string<N> const& x, std::string_view const& y) {
        return x.compare(y) != 0;
    }


    template<std::size_t N>
    bool operator>=(std::string_view const& x, fixed_string<N> const& y) {
        return y.compare(x) != 0;
    }


    template<typename OS, std::size_t N>
    OS& operator<<(OS& stream, fixed_string<N> const& fs) {
        stream << std::string_view {fs.data(), fs.size()};
        return stream;
    }


}   // namespace nonstd



namespace std {


    template<size_t N>
    struct hash<ufmt::fixed_string<N>> {
        size_t operator()(ufmt::fixed_string<N> const& s) const {
            size_t h = 0;
            char const* cc = s.data();
            while(*cc)
                h = h * 101 + static_cast<size_t>(*cc++);
            return h;
        }

    };   // hash


}   // namespace std
