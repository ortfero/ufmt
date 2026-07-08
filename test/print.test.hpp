#pragma once


#include "doctest.h"

#include <ufmt/print.hpp>


TEST_SUITE("print") {


    SCENARIO("print_with forwards an rvalue result") {
        int const r = ufmt::print_with(42, "print_with rvalue: ", 1);
        REQUIRE_EQ(r, 42);
    }


    SCENARIO("print_with forwards an lvalue result by reference") {
        int value = 7;
        int& r = ufmt::print_with(value, "print_with lvalue: ", 2);
        REQUIRE_EQ(&r, &value);
        REQUIRE_EQ(r, 7);
    }


    SCENARIO("error_with forwards its result") {
        int const r = ufmt::error_with(99, "error_with: ", 3);
        REQUIRE_EQ(r, 99);
    }

}
