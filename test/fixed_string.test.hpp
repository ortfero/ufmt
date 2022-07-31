#pragma once

#include "doctest.h"

#include <sstream>
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

}
