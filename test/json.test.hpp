#pragma once


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
        REQUIRE_EQ(ufmt::json::of("x", std::vector{1, 2, 3}), R"({"x":[1,2,3]})");
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
        REQUIRE_EQ(ufmt::json::of("point", point{-1, -1}), R"({"point":{"x":-1,"y":-1}})");
    }
    
}
