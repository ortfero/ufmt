/* This file is part of uformat library
 * Copyright 2020 Andrei Ilin <ortfero@gmail.com>
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


#include <string>
#include <string_view>


namespace uformat::utf8 {


#if defined(_WIN32)
    inline static constexpr auto octets_per_wchar = 3;
#else
    inline static constexpr auto octets_per_wchar = 4;
#endif


    inline constexpr std::size_t
        octets_buffer_size(std::size_t code_point_count) noexcept {
            return octets_per_wchar * code_point_count;
    }


    inline constexpr void code_point_to_octets(wchar_t const*& p,
                                               char*& q) noexcept {
        wchar_t const cp = *p;

        if(cp < 0x80) {
            *q++ = char(cp);
        } else if(cp < 0x800) {
            *q++ = char((cp >> 6) | 0xc0);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#if defined(_WIN32)
        else {
            *q++ = char((cp >> 12) | 0xe0);
            *q++ = char((cp >> 6) | 0x3f);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#else
        else if(cp < 0x10000) {
            *q++ = char((cp >> 12) | 0xe0);
            *q++ = char((cp >> 6) | 0x3f);
            *q++ = char((cp & 0x3f) | 0x80);
        } else {
            *q++ = char((cp >> 18) | 0xf0);
            *q++ = char(((cp >> 12) & 0x3f) | 0x80);
            *q++ = char(((cp >> 6) & 0x3f) | 0x80);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#endif
        ++p;
    }


    inline constexpr bool code_point_to_octets(wchar_t const*& p,
                                               char*& q,
                                               char const* end) noexcept {
        wchar_t const cp = *p;
        if(cp < 0x80) {
            if(end - q < 1)
                return false;
            *q++ = char(cp);
        } else if(cp < 0x800) {
            if(end - q < 2)
                return false;
            *q++ = char((cp >> 6) | 0xc0);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#if defined(_WIN32)
        else {
            if(end - q < 3)
                return false;
            *q++ = char((cp >> 12) | 0xe0);
            *q++ = char((cp >> 6) | 0x3f);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#else
        else if(cp < 0x10000) {
            if(end - q < 3)
                return false;
            *q++ = char((cp >> 12) | 0xe0);
            *q++ = char((cp >> 6) | 0x3f);
            *q++ = char((cp & 0x3f) | 0x80);
        } else {
            if(end - q < 4)
                return false;
            *q++ = char((cp >> 18) | 0xf0);
            *q++ = char(((cp >> 12) & 0x3f) | 0x80);
            *q++ = char(((cp >> 6) & 0x3f) | 0x80);
            *q++ = char((cp & 0x3f) | 0x80);
        }
#endif
        ++p;
        return true;
    }


    inline void to(wchar_t const* p, char*& q) noexcept {
        if(p == nullptr) {
            *q = '\0';
            return;
        }
        while(*p != '\0')
            code_point_to_octets(p, q);
        *q = '\0';
    }


    inline void to(wchar_t const* p, char*& q, char const* end) noexcept {
        if(p == nullptr) {
            *q = '\0';
            return;
        }
        while(*p != '\0')
            if(!code_point_to_octets(p, q, end - 1))
                break;
        *q = '\0';
    }


    inline void to(std::wstring_view const& wsv, char*& q) noexcept {
        wchar_t const* p = wsv.data();
        while(p != wsv.data() + wsv.size())
            code_point_to_octets(p, q);
        *q = '\0';
    }


    inline void
        to(std::wstring_view const& wsv, char*& q, char const* end) noexcept {
        wchar_t const* p = wsv.data();
        while(p != wsv.data() + wsv.size())
            if(!code_point_to_octets(p, q, end - 1))
                break;
        *q = '\0';
    }


    template<std::size_t N>
    void to(wchar_t const (&literal)[N], char*& q) noexcept {
        to(std::wstring_view {literal, N - 1}, q);
    }


    template<std::size_t N>
    void to(wchar_t const (&literal)[N], char*& q, char const* end) noexcept {
        to(std::wstring_view {literal, N - 1}, q, end);
    }


    inline void to(std::wstring const& ws, std::string& s) {
        if(ws.empty()) {
            return s.clear();
        }
        s.resize(octets_buffer_size(ws.size()));
        char* q = s.data();
        to(ws.data(), q);
        s.resize(q - s.data());
    }


    inline std::string to(std::wstring const& ws) {
        std::string s;
        to(ws, s);
        return s;
    }


    inline void to(std::wstring_view const& wsv, std::string& s) {
        if(wsv.empty()) {
            return s.clear();
        }
        s.resize(octets_buffer_size(wsv.size()));
        char* q = s.data();
        to(wsv, q);
        s.resize(q - s.data());
    }


    inline std::string to(std::wstring_view const& wsv) {
        std::string s;
        to(wsv, s);
        return s;
    }

}   // namespace uformat::utf8
