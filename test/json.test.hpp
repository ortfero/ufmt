#pragma once


#include <cmath>
#include <limits>
#include <optional>
#include <string_view>

#include "doctest.h"

#include <ufmt/json.hpp>


TEST_SUITE("json") {
    
    SCENARIO("Format i32") {
        REQUIRE_EQ(ufmt::json::of("x", -1), R"({"x":-1})");
    }
    
    SCENARIO("Format double") {
        REQUIRE_EQ(ufmt::json::of("x", 3.14), R"({"x":3.14})");
    }
    
    SCENARIO("Format string") {
        REQUIRE_EQ(ufmt::json::of("x", "ok"), R"({"x":"ok"})");
        REQUIRE_EQ(ufmt::json::of("x", std::string{"ok"}), R"({"x":"ok"})");
        REQUIRE_EQ(ufmt::json::of("x", std::string_view{"ok"}), R"({"x":"ok"})");
    }
    
    SCENARIO("Format array") {
        REQUIRE_EQ(ufmt::json::of(std::vector<int>{}), R"([])");
        REQUIRE_EQ(ufmt::json::of(std::vector{1, 2, 3}), R"([1,2,3])");
    }
    
    SCENARIO("Format object") {
        REQUIRE_EQ(ufmt::json::of("x", -1, "y", 3.14, "z", "ok"), R"({"x":-1,"y":3.14,"z":"ok"})");
    }
    
    struct point {
        int x, y;
    };
    
    template<class S> ufmt::basic_json<S>& operator << (ufmt::basic_json<S>& json, point const& p) {
        return json << ufmt::object("x", p.x, "y", p.y);
    }
    
    SCENARIO("Format custom type") {
        REQUIRE_EQ(ufmt::json::of(point{-1, -2}), R"({"x":-1,"y":-2})");
    }
    
    struct point3d {
        int x, y;
        std::optional<int> z;
    };
    
    template<class S> ufmt::basic_json<S>& operator << (ufmt::basic_json<S>& json, point3d const& p) {
        return json << ufmt::object("x", p.x, "y", p.y, "z", p.z);
    }
    
    SCENARIO("Format with optional fields") {
        REQUIRE_EQ(ufmt::json::of(point3d{-1, -2, -3}), R"({"x":-1,"y":-2,"z":-3})");
        REQUIRE_EQ(ufmt::json::of(point3d{-1, -2, std::nullopt}), R"({"x":-1,"y":-2})");
    }


    SCENARIO("Escape special characters") {
        REQUIRE_EQ(ufmt::json::of("x", "a\"b"), R"({"x":"a\"b"})");
        REQUIRE_EQ(ufmt::json::of("x", "a\\b"), R"({"x":"a\\b"})");
        REQUIRE_EQ(ufmt::json::of("x", "a\nb"), R"({"x":"a\nb"})");
        REQUIRE_EQ(ufmt::json::of("x", "a\tb"), R"({"x":"a\tb"})");
        REQUIRE_EQ(ufmt::json::of("x", std::string_view{"\x01", 1}), R"({"x":"\u0001"})");
    }


    SCENARIO("Non-finite double becomes null") {
        REQUIRE_EQ(ufmt::json::of("x", std::numeric_limits<double>::infinity()),
                   R"({"x":null})");
        REQUIRE_EQ(ufmt::json::of("x", std::numeric_limits<double>::quiet_NaN()),
                   R"({"x":null})");
    }


    SCENARIO("Wide integer values") {
        REQUIRE_EQ(ufmt::json::of("x", -1234567890123LL), R"({"x":-1234567890123})");
        REQUIRE_EQ(ufmt::json::of("x", std::size_t(42)), R"({"x":42})");
    }


    struct opt_first {
        std::optional<int> a;
        int b;
    };

    template<class S> ufmt::basic_json<S>& operator << (ufmt::basic_json<S>& json, opt_first const& v) {
        return json << ufmt::object("a", v.a, "b", v.b);
    }

    SCENARIO("Optional as first field") {
        REQUIRE_EQ(ufmt::json::of(opt_first{5, 7}), R"({"a":5,"b":7})");
        REQUIRE_EQ(ufmt::json::of(opt_first{std::nullopt, 7}), R"({"b":7})");
    }

}
