#pragma once


#include "doctest.h"

#include <ufmt/text.hpp>


TEST_SUITE("text") {
    
    
    SCENARIO("Construct") {
        ufmt::text target;
        REQUIRE(target.size() == 0);
        REQUIRE(target.empty());
        REQUIRE_EQ(target[0], '\0');
    }


    SCENARIO("Format character") {
        REQUIRE_EQ(ufmt::text::of('a'), "a");
    }


    SCENARIO("Clear formatter") {
        ufmt::text target;
        target << "qwerty";
        target.clear();
        REQUIRE_EQ(target.size(), 0);
        REQUIRE(target.empty());
        REQUIRE_EQ(target[0], '\0');
    }


    SCENARIO("Format string literal") {
        REQUIRE_EQ(ufmt::text::of("qwerty"), "qwerty");
    }


    SCENARIO("Format integer") {
        REQUIRE_EQ(ufmt::text::of(127562), "127562");
    }


    SCENARIO("Format fixed integer") {
        using ufmt::fixed;
        REQUIRE_EQ(ufmt::text::of(fixed(127, 2)), "127");
        REQUIRE_EQ(ufmt::text::of(fixed(12, 4)), "0012");
    }


    SCENARIO("Format double") {
        REQUIRE_EQ(ufmt::text::of(1.0), "1");
        REQUIRE_EQ(ufmt::text::of(0.009990), "0.00999");
    }


    SCENARIO("Format double with fixed precision") {
        using ufmt::precised;
        REQUIRE_EQ(ufmt::text::of(precised(1.0, 3)), "1.000");
    }


    SCENARIO("Format quoted") {
        using ufmt::quoted;
        REQUIRE_EQ(ufmt::text::of(quoted(127562)), "'127562'");
    }


    SCENARIO("Format double quoted") {
        using ufmt::dquoted;
        REQUIRE_EQ(ufmt::text::of(dquoted(127562)), "\"127562\"");
    }


    SCENARIO("Format char_n") {
        auto target = ufmt::text {};
        target << ufmt::char_n('*', 7);
        REQUIRE_EQ(target.string(), "*******");
    }


    SCENARIO("Align left") {
        using ufmt::left;
        REQUIRE_EQ(ufmt::text::of(left(-1, 4)), "-1  ");
        REQUIRE_EQ(ufmt::text::of(left(-10, 2)), "-10");
    }


    SCENARIO("Align right") {
        using ufmt::right;
        REQUIRE_EQ(ufmt::text::of(right(-1, 4)), "  -1");
        REQUIRE_EQ(ufmt::text::of(right(-10, 2)), "-10");
    }


    SCENARIO("Format uint64_t") {
        auto target = ufmt::text{};
        target << std::uint64_t(0xFFFFFFFFFFFFFFFFull);
        REQUIRE_EQ(target.string(), "18446744073709551615");
    }
        
    
    SCENARIO("Move internal buffer") {
        auto target = ufmt::text{};
        target << -1;
        auto const s1 = std::move(target).string();
        std::cout << target.string().size() << std::endl;
        REQUIRE(target.string().empty());
        REQUIRE_EQ(s1, "-1");
    }
}
