#pragma once


#include "doctest.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

#include <ufmt/buffered_file.hpp>


TEST_SUITE("buffered_file") {


    static std::string slurp_bf(std::string const& path) {
        std::ifstream in(path, std::ios::binary);
        std::ostringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }


    SCENARIO("create_always writes formatted content via print") {
        std::string const path = "/tmp/ufmt_buffered_file_print.txt";
        std::remove(path.c_str());
        std::error_code ec;
        {
            auto f = ufmt::buffered_file<>::create_always(path, ec);
            REQUIRE(!ec);
            REQUIRE(f.has_value());
            f->print("a=", 3, "!");
        }
        REQUIRE_EQ(slurp_bf(path), "a=3!\n");
        std::remove(path.c_str());
    }


    SCENARIO("write emits the given string_view") {
        std::string const path = "/tmp/ufmt_buffered_file_write.txt";
        std::remove(path.c_str());
        std::error_code ec;
        {
            auto f = ufmt::buffered_file<>::create_always(path, ec);
            REQUIRE(f.has_value());
            bool const ok = f->write("hello", ec);
            REQUIRE(ok);
            REQUIRE(!ec);
        }
        REQUIRE_EQ(slurp_bf(path), "hello");
        std::remove(path.c_str());
    }


    SCENARIO("open_existing on a missing file reports an error") {
        std::error_code ec;
        auto f = ufmt::buffered_file<>::open_existing(
            "/tmp/ufmt_buffered_file_absent_zzz.txt", ec);
        REQUIRE(!f.has_value());
        REQUIRE(bool(ec));
    }

}
