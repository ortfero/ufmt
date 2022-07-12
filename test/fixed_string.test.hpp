#pragma once

#include "doctest.h"

#include <sstream>
#include <ufmt/fixed_string.hpp>


TEST_SUITE("fixed_string") {


    TEST_CASE("fixed_string::fixed_string") {
        ufmt::string target;
        REQUIRE(target.empty());
        REQUIRE(target.size() == 0);
        REQUIRE(target.length() == 0);
    }


    TEST_CASE("fixed_string(\"test\")") {
        ufmt::string target("test");
        REQUIRE(!target.empty());
        REQUIRE(target.size() == sizeof("test") - 1);
        REQUIRE(target.length() == sizeof("test") - 1);
    }


    TEST_CASE("operator << (ostream, fixed_string)") {
        std::stringstream ss;
        ufmt::string target("test");
        ss << target;
        REQUIRE(ss.str() == "test");
    }
    
    
    TEST_CASE("conversion from wchar_t*") {
        static wchar_t const* data = L"1234";
        auto const target = ufmt::string{ data };
        REQUIRE_EQ(target.size(), 4);
        auto const equals = strcmp(target.data(), "1234") == 0;
        REQUIRE(equals);
    }

}
