/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/classes/alternative.hpp"
#include "fun/instances/alternative.hpp"

using namespace fun;
using namespace fun::underscore;

TEST_CASE("alternative"){
    SECTION("empty") {
        maybe_t<int> m = alternative_f::empty<maybe_t, int>();
        REQUIRE(m.is_nothing());

        maybe_t<int> m2 = alternative_f::fempty<maybe_t, int>;
        REQUIRE(m2.is_nothing());
    }
    SECTION("alter") {
        {
            maybe_t<int> m = alternative_f::alter(make_just(42), make_just(10));
            REQUIRE(*m == 42);
            maybe_t<int> m1 = alternative_f::alter(make_nothing<int>(), make_just(10));
            REQUIRE(*m1 == 10);
            maybe_t<int> m2 = alternative_f::alter(make_just(42), make_nothing<int>());
            REQUIRE(*m2 == 42);
            maybe_t<int> m3 = alternative_f::alter(make_nothing<int>(), make_nothing<int>());
            REQUIRE(m3.is_nothing());
        }
        {
            maybe_t<int> m = alternative_f::falter(make_just(42), make_just(10));
            REQUIRE(*m == 42);
            maybe_t<int> m1 = alternative_f::falter(make_nothing<int>(), make_just(10));
            REQUIRE(*m1 == 10);
            maybe_t<int> m2 = alternative_f::falter(make_just(42), make_nothing<int>());
            REQUIRE(*m2 == 42);
            maybe_t<int> m3 = alternative_f::falter(make_nothing<int>(), make_nothing<int>());
            REQUIRE(m3.is_nothing());
        }
        {
            using namespace fun::alternative_ops;

            maybe_t<int> m = make_just(42) || make_just(10);
            REQUIRE(*m == 42);
            maybe_t<int> m1 = make_nothing<int>() || make_just(10);
            REQUIRE(*m1 == 10);
            maybe_t<int> m2 = make_just(42) || make_nothing<int>();
            REQUIRE(*m2 == 42);
            maybe_t<int> m3 = make_nothing<int>() || make_nothing<int>();
            REQUIRE(m3.is_nothing());
        }
    }
}
