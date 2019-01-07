/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/classes/applicative.hpp"
#include "fun/instances/applicative.hpp"

using namespace fun;
using namespace fun::underscore;

TEST_CASE("applicative"){
    SECTION("pure") {
        box_t<int> b = applicative_f::pure<box_t>(42);
        REQUIRE(*b == 42);

        box_t<int> b2 = applicative_f::fpure<box_t>(42);
        REQUIRE(*b2 == 42);

        maybe_t<int> m = applicative_f::pure<maybe_t>(42);
        REQUIRE(m.is_just());
        REQUIRE(*m == 42);

        maybe_t<int> m2 = applicative_f::fpure<maybe_t>(42);
        REQUIRE(m2.is_just());
        REQUIRE(*m2 == 42);
    }
    SECTION("apply") {
        box_t<int> b = applicative_f::apply(make_box(_ + 2), make_box(40));
        REQUIRE(*b == 42);
        box_t<int> b2 = applicative_f::fapply(make_box(_ + 2), make_box(40));
        REQUIRE(*b2 == 42);

        maybe_t<int> m = applicative_f::apply(make_just(_ + 2), make_just(40));
        REQUIRE(*m == 42);
        maybe_t<int> m2 = applicative_f::apply(make_nothing<int(int)>(), make_just(40));
        REQUIRE(m2.is_nothing());
        maybe_t<int> m3 = applicative_f::apply(make_just(_ + 2), make_nothing<int>());
        REQUIRE(m3.is_nothing());
        maybe_t<int> m4 = applicative_f::apply(make_nothing<int(int)>(), make_nothing<int>());
        REQUIRE(m4.is_nothing());

        maybe_t<int> m5 = applicative_f::fapply(make_just(_ + 2), make_just(40));
        REQUIRE(*m5 == 42);
        maybe_t<int> m6 = applicative_f::fapply(make_nothing<int(int)>(), make_just(40));
        REQUIRE(m6.is_nothing());
        maybe_t<int> m7 = applicative_f::fapply(make_just(_ + 2), make_nothing<int>());
        REQUIRE(m7.is_nothing());
        maybe_t<int> m8 = applicative_f::fapply(make_nothing<int(int)>(), make_nothing<int>());
        REQUIRE(m8.is_nothing());

        maybe_t<int> m9 = make_just(_ + 20) >>= make_just(22);
        REQUIRE(*m == 42);
    }
    SECTION("lift_a2") {
        box_t<int> b = applicative_f::lift_a2(_/_, make_box(84), make_box(2));
        REQUIRE(*b == 42);
        box_t<int> b2 = applicative_f::flift_a2(_/_, make_box(84), make_box(2));
        REQUIRE(*b2 == 42);

        maybe_t<int> m = applicative_f::lift_a2(_*_, make_just(21), make_just(2));
        REQUIRE(*m == 42);
        REQUIRE(applicative_f::lift_a2(_*_, make_nothing<int>(), make_just(2)).is_nothing());
        REQUIRE(applicative_f::lift_a2(_*_, make_just(21), make_nothing<int>()).is_nothing());
        REQUIRE(applicative_f::lift_a2(_*_, make_nothing<int>(), make_nothing<int>()).is_nothing());
    }
    SECTION("lift_a3") {
        const auto f = [](int a, int b, int c) {
            return a + b + c;
        };

        box_t<int> b = applicative_f::lift_a3(f, make_box(84), make_box(2), make_box(-44));
        REQUIRE(*b == 42);
        box_t<int> b2 = applicative_f::flift_a3(f, make_box(84), make_box(2), make_box(-44));
        REQUIRE(*b2 == 42);

        maybe_t<int> m = applicative_f::lift_a3(f, make_just(39), make_just(2), make_just(1));
        REQUIRE(*m == 42);
        REQUIRE(applicative_f::lift_a3(f, make_nothing<int>(), make_just(2), make_just(3)).is_nothing());
        REQUIRE(applicative_f::lift_a3(f, make_just(2), make_nothing<int>(), make_just(3)).is_nothing());
        REQUIRE(applicative_f::lift_a3(f, make_just(2), make_just(3), make_nothing<int>()).is_nothing());
    }
}
