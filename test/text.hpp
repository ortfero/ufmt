#pragma once


#include "doctest.h"

#include <ufmt/text.hpp>


TEST_SUITE("text") {
    TEST_CASE("constructor") {
        ufmt::text target;
        REQUIRE(target.size() == 0);
        REQUIRE(target.empty());
        REQUIRE_EQ(target[0], '\0');
    }


    TEST_CASE("format(char)") { REQUIRE_EQ(ufmt::text::of('a').string(), "a"); }


    TEST_CASE("clear") {
        ufmt::text target;
        target << "qwerty";
        target.clear();
        REQUIRE_EQ(target.size(), 0);
        REQUIRE(target.empty());
        REQUIRE_EQ(target[0], '\0');
    }


    TEST_CASE("format(char[])") {
        REQUIRE_EQ(ufmt::text::of("qwerty").string(), "qwerty");
    }


    TEST_CASE("format(int)") {
        REQUIRE_EQ(ufmt::text::of(127562).string(), "127562");
    }


    TEST_CASE("fixed(unsigned)") {
        using ufmt::fixed;
        REQUIRE_EQ(ufmt::text::of(fixed(127, 2)).string(), "127");
        REQUIRE_EQ(ufmt::text::of(fixed(12, 4)).string(), "0012");
    }


    TEST_CASE("format(double)") {
        REQUIRE_EQ(ufmt::text::of(1.0).string(), "1");
        REQUIRE_EQ(ufmt::text::of(0.009990).string(), "0.00999");
    }


    TEST_CASE("precised(double)") {
        using ufmt::precised;
        REQUIRE_EQ(ufmt::text::of(precised(1.0, 3)).string(), "1.000");
    }


    TEST_CASE("quoted") {
        using ufmt::quoted;
        REQUIRE_EQ(ufmt::text::of(quoted(127562)).string(), "'127562'");
    }


    TEST_CASE("dquoted") {
        using ufmt::dquoted;
        REQUIRE_EQ(ufmt::text::of(dquoted(127562)).string(), "\"127562\"");
    }


    TEST_CASE("char_n") {
        auto target = ufmt::text {};
        target.char_n('*', 7);
        REQUIRE_EQ(target.string(), "*******");
    }


    TEST_CASE("left") {
        using ufmt::left;
        REQUIRE_EQ(ufmt::text::of(left(-1, 4)).string(), "-1  ");
        REQUIRE_EQ(ufmt::text::of(left(-10, 2)).string(), "-10");
    }


    TEST_CASE("right") {
        using ufmt::right;
        REQUIRE_EQ(ufmt::text::of(right(-1, 4)).string(), "  -1");
        REQUIRE_EQ(ufmt::text::of(right(-10, 2)).string(), "-10");
    }


    TEST_CASE("uint64_t") {
        auto target = ufmt::text{};
        target << std::uint64_t(0xFFFFFFFFFFFFFFFFull);
        REQUIRE_EQ(target.string(), "18446744073709551615");
    }
}
