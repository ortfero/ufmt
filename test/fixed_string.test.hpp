#pragma once

#include "doctest.h"

#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <ufmt/fixed_string.hpp>


TEST_SUITE("fixed_string") {


    SCENARIO("Default construction") {
        ufmt::string target;
        REQUIRE(target.empty());
        REQUIRE(target.size() == 0);
        REQUIRE(target.length() == 0);
    }


    SCENARIO("Construction from string literal") {
        ufmt::string target("test");
        REQUIRE(!target.empty());
        REQUIRE(target.size() == sizeof("test") - 1);
        REQUIRE(target.length() == sizeof("test") - 1);
    }


    SCENARIO("Output to ostream") {
        std::stringstream ss;
        ufmt::string target("test");
        ss << target;
        REQUIRE(ss.str() == "test");
    }
    
    
    SCENARIO("Conversion from wchar_t*") {
        static wchar_t const* data = L"1234";
        auto const target = ufmt::string{ data };
        REQUIRE_EQ(target.size(), 4);
        auto const equals = strcmp(target.data(), "1234") == 0;
        REQUIRE(equals);
    }


    SCENARIO("Copy to wchar_t array") {
        auto const text = ufmt::short_string{"text"};
        auto target = std::array<wchar_t, 4>{};
        text.copy_to(target);
        REQUIRE_EQ(target[0], L't');
        REQUIRE_EQ(target[1], L'e');
        REQUIRE_EQ(target[2], L'x');
        REQUIRE_EQ(target[3], L'\0');
    }


    SCENARIO("Equality across types") {
        ufmt::string const a{"hello"};
        REQUIRE(a == ufmt::string{"hello"});
        REQUIRE(a == "hello");
        REQUIRE("hello" == a);
        REQUIRE(a == std::string{"hello"});
        REQUIRE(a == std::string_view{"hello"});
        REQUIRE(a != ufmt::string{"world"});
        REQUIRE(a != "help");
    }


    SCENARIO("Ordering") {
        REQUIRE(ufmt::string{"abc"} < ufmt::string{"abd"});
        REQUIRE(ufmt::string{"abc"} > ufmt::string{"abb"});
        REQUIRE(ufmt::string{"ab"} < ufmt::string{"abc"});    // prefix sorts first
        REQUIRE(ufmt::string{"abc"} > ufmt::string{"ab"});
        REQUIRE(ufmt::string{"abc"} <= ufmt::string{"abc"});
        REQUIRE(ufmt::string{"abc"} >= ufmt::string{"abc"});
        REQUIRE(ufmt::string{"abc"} < "abd");
        REQUIRE(ufmt::string{"abc"} < std::string_view{"abd"});
    }


    SCENARIO("Ordering across sizes") {
        ufmt::fixed_string<8> const a{"abc"};
        ufmt::fixed_string<32> const b{"abd"};
        REQUIRE(a < b);
        REQUIRE(b > a);
        REQUIRE(a == ufmt::fixed_string<64>{"abc"});
    }


    SCENARIO("Compare prefix vs full") {
        REQUIRE(ufmt::string{"ab"}.compare("abc") < 0);
        REQUIRE(ufmt::string{"abc"}.compare("ab") > 0);
        REQUIRE(ufmt::string{"abc"}.compare("abc") == 0);
    }


    SCENARIO("find_last_of") {
        ufmt::string const s{"a.b.c"};
        REQUIRE_EQ(s.find_last_of('.'), std::size_t(3));
        REQUIRE_EQ(s.find_last_of('x'), ufmt::string::npos);
        ufmt::string const empty;
        REQUIRE_EQ(empty.find_last_of('.'), ufmt::string::npos);
    }


    SCENARIO("Hash distinguishes content after embedded NUL") {
        ufmt::string a;
        a.append("x\0a", 3);
        ufmt::string b;
        b.append("x\0b", 3);
        REQUIRE_EQ(a.size(), std::size_t(3));
        std::hash<ufmt::string> const h;
        REQUIRE(h(a) != h(b));
    }

}
