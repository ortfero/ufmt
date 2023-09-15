// This file is part of ufmt library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <tuple>
#include <vector>

#include <ufmt/text.hpp>
#include <ufmt/fixed_string.hpp>


namespace ufmt {
    
    template<typename T> struct field {
        std::string_view name;
        T const& value;
        
        template<std::size_t N> constexpr field(char const (&name)[N], T const& value) noexcept
        : name{name, N - 1}, value{value} {            
        }
    }; // field
    
    
    inline constexpr std::tuple<> object() noexcept {
        return std::tuple<>{};
    }
    
    
    template<std::size_t N, typename Arg, typename... Args> auto constexpr
    object(char const (&name)[N], Arg&& arg, Args&&... args) noexcept {
        return tuple_cat(std::tuple<field<Arg>>{field<Arg>{name, std::forward<Arg>(arg)}},
                         object(std::forward<Args>(args)...));
    }
     
    
    template<class S>
    class basic_json {
        basic_text<S> text_;
        
    public:
    
        using size_type = typename basic_text<S>::size_type;
        using value_type = typename S::value_type;

        static S of() {
            return S{"{}"};
        }


        template<std::size_t N, typename Arg, typename... Args>
        static S of(char const (&name)[N], Arg&& arg, Args&&... args) {
            basic_json j;
            j << ufmt::object(name, std::forward<Arg>(arg), std::forward<Args>(args)...);
            return j.string();
        }


        template<typename T>
        static S of(T const& object) {
            basic_json j;
            j << object;
            return j.string();
        }

    
        S const& string() const & noexcept { return text_.string(); }
        S&& string() && noexcept { return std::move(text_).string(); }
        value_type const* data() const noexcept { return text_.data(); }
        size_type size() const noexcept { return text_.size(); }
        bool empty() const noexcept { return text_.empty(); }
        void clear() noexcept { text_.clear(); }
        size_type capacity() const noexcept { return text_.capacity(); }
        std::string_view view() const noexcept { return text_.view(); }
        void reserve(size_type n) { text_.reserve(n); }
                
        basic_json& operator << (std::int32_t arg) {
            text_ << arg;
            return *this;
        }

        
        basic_json& operator << (std::uint32_t arg) {
            text_ << arg;
            return *this;
        }

                
        basic_json& operator << (std::int64_t arg) {
            text_ << arg;
            return *this;
        }

        
        basic_json& operator << (std::uint64_t arg) {
            text_ << arg;
            return *this;
        }

        
        basic_json& operator << (double arg) {
            text_ << arg;
            return *this;
        }


        basic_json& operator << (formatters::precised<double> arg) {
            text_ << arg;
            return *this;
        }

        
        basic_json& operator << (bool arg) {
            if(arg)
                text_ << "true";
            else
                text_ << "false";
            return *this;
        }

        
        basic_json& operator << (std::nullptr_t) {
            text_ << "null";
            return *this;
        }

        
        template<std::size_t N> basic_json& operator << (char const (&arg)[N]) {
            text_ << '\"' << arg << '\"';
            return *this;
        }

        
        basic_json& operator << (std::string_view arg) {
            text_ << '\"' << arg << '\"';
            return *this;
        }

        
        basic_json& operator << (char arg) {
            text_ << '\"' << arg << '\"';
            return *this;
        }

        
        basic_json& operator << (std::string const& arg) {
            text_ << '\"' << arg << '\"';
            return *this;
        }

        
        template<std::size_t N> basic_json& operator << (fixed_string<N> const& arg) {
            text_ << '\"' << arg << '\"';
            return *this;
        }
        
        
        template<typename T, std::size_t N> basic_json& operator << (std::array<T, N> const& arg) {
            return format_array(arg);
        }
        
        
        template<typename T> basic_json& operator << (std::vector<T> const& arg) {
            return format_array(arg);
        }
        
        
        template<typename... Args> basic_json& operator << (std::tuple<> const& ) {
            text_ << "{}";
            return *this;
        }
        
        
        template<typename Arg, typename... Args> basic_json& operator << (std::tuple<field<Arg>, field<Args>...> const& arg) {
            text_ << "{\"" << std::get<0>(arg).name << "\":";
            (*this) << std::get<0>(arg).value;
            format_object(std::apply([](auto&&, auto&&... args) { return std::tie(args...);}, arg));
            text_ << '}';
            return *this;
        }
        
        
        // template<typename T> basic_json& operator << (std::span<T> arg) {
            // return format_array(arg);
        // }
        
        
    private:
    
        template<class C> basic_json& format_array(C const& arg) {
            text_ << '[';
            if(!arg.empty()) {
                auto it = arg.begin();
                (*this) << *it;
                ++it;
                while(it != arg.end()) {
                    text_ << ',';
                    (*this) << *it;
                    ++it;
                }                
            }
            text_ << ']';
            return *this;
        }
        
        void format() {
        }

        template<std::size_t N, typename Arg, typename... Args>
        void format(char const (&name)[N], Arg&& arg, Args&&... args) {
            text_ << ",\"" << name << "\":";
            (*this) << std::forward<Arg>(arg);
            format(std::forward<Args>(args)...);
        }


        void format_object(std::tuple<> const&) {
        }        

        template<typename Arg, typename... Args>
        void format_object(std::tuple<field<Arg> const&, field<Args> const&...> const& arg) {
            format_field(std::get<0>(arg));
            format_object(std::apply([](auto&&, auto&&... args) { return std::tie(args...); }, arg));
        }
        

        template<typename Arg>
        void format_field(field<Arg> f) {
            text_ << ',' << '\"' << f.name << '\"' << ':';
            (*this) << f.value; 
        }

        template<typename Arg>
        void format_field(field<std::optional<Arg> const&> f) {
            if(!f.value.has_value())
                return;
            text_ << ',' << '\"' << f.name << '\"' << ':';
            (*this) << *f.value;
        }
        
    }; // basic_json
    
    
    using json = basic_json<std::string>;
    using long_string_json = basic_json<ufmt::long_string>;
    using page_string_json = basic_json<ufmt::page_string>;
    using double_page_string_json = basic_json<ufmt::double_page_string>;
    using large_string_json = basic_json<ufmt::large_string>;
    
    
} // namespace ufmt
