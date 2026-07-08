#pragma once


#include "doctest.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

#include <ufmt/text_file.hpp>


TEST_SUITE("text_file") {


    static std::string slurp_tf(std::string const& path) {
        std::ifstream in(path, std::ios::binary);
        std::ostringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }


    SCENARIO("create_always writes formatted content via print") {
        std::string const path = "/tmp/ufmt_text_file_print.txt";
        std::remove(path.c_str());
        std::error_code ec;
        {
            auto f = ufmt::text_file<>::create_always(path, ec);
            REQUIRE(!ec);
            REQUIRE(f.has_value());
            f->print("x=", 1, " y=", 2.5);
        }
        REQUIRE_EQ(slurp_tf(path), "x=1 y=2.5\n");
        std::remove(path.c_str());
    }


    SCENARIO("write emits the given string_view, not the internal buffer") {
        std::string const path = "/tmp/ufmt_text_file_write.txt";
        std::remove(path.c_str());
        std::error_code ec;
        {
            auto f = ufmt::text_file<>::create_always(path, ec);
            REQUIRE(f.has_value());
            bool const ok = f->write("hello world", ec);
            REQUIRE(ok);
            REQUIRE(!ec);
        }
        REQUIRE_EQ(slurp_tf(path), "hello world");
        std::remove(path.c_str());
    }


    SCENARIO("open_existing on a missing file reports an error") {
        std::error_code ec;
        auto f = ufmt::text_file<>::open_existing(
            "/tmp/ufmt_text_file_absent_zzz.txt", ec);
        REQUIRE(!f.has_value());
        REQUIRE(bool(ec));
    }

}
