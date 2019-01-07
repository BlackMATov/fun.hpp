/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/instances/eq.hpp"

#include "fun/classes/monad.hpp"
#include "fun/instances/monad.hpp"

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::underscore;

TEST_CASE("monad"){
    SECTION("mreturn") {
        REQUIRE((monad_f::mreturn<maybe_t>(10) == make_just(10)));
    }
    SECTION("fmreturn") {
        REQUIRE((monad_f::fmreturn<maybe_t>(10) == make_just(10)));
    }
    SECTION("mbind") {
        REQUIRE((monad_f::mbind(fjust(10), fjust * (_*2)) == fjust(20)));
        REQUIRE((monad_f::mbind(fnothing<int>, fjust * (_*2)) == fnothing<int>));
    }
    SECTION("fmbind") {
        REQUIRE((monad_f::fmbind(fjust(10), fjust * (_*2)) == fjust(20)));
        REQUIRE((monad_f::fmbind(fnothing<int>, fjust * (_*2)) == fnothing<int>));
    }
    SECTION("mbind_const") {
        REQUIRE((monad_f::mbind_const(fjust(10), fjust(20)) == make_just(20)));
        REQUIRE((monad_f::mbind_const(fnothing<int>, fjust(20)) == fnothing<int>));
    }
    SECTION("fmbind_const") {
        REQUIRE((monad_f::fmbind_const(fjust(10), fjust(20)) == make_just(20)));
        REQUIRE((monad_f::fmbind_const(fnothing<int>, fjust(20)) == fnothing<int>));
    }
    SECTION("operators") {
        using namespace monad_ops;

        REQUIRE(((fjust(10) >>= fjust * (_*2)) == fjust(20)));
        REQUIRE(((fnothing<int> >>= fjust * (_*2)) == fnothing<int>));

        REQUIRE(((fjust(10) >> fjust(20)) == fjust(20)));
        REQUIRE(((fnothing<int> >> fjust(20)) == fnothing<int>));
    }
    SECTION("lifts") {
        const auto f = [](int a, int b, int c) {
            return a + b + c;
        };
        REQUIRE((monad_f::lift_m((_+10), fjust(32)) == fjust(42)));
        REQUIRE((monad_f::lift_m2((_+_), fjust(32), fjust(10)) == fjust(42)));
        REQUIRE((monad_f::lift_m3(f, fjust(30), fjust(10), fjust(2)) == fjust(42)));
    }
    SECTION("flifts") {
        const auto f = [](int a, int b, int c) {
            return a + b + c;
        };
        REQUIRE((monad_f::flift_m((_+10), fjust(32)) == fjust(42)));
        REQUIRE((monad_f::flift_m2((_+_), fjust(32), fjust(10)) == fjust(42)));
        REQUIRE((monad_f::flift_m3(f, fjust(30), fjust(10), fjust(2)) == fjust(42)));
    }

    SECTION("mdo"){
        using namespace monad_f;
        {
            {
                auto m = mdo<maybe_t>(
                    [](){ return fjust(12); });
                REQUIRE(m.is_just());
                REQUIRE(*m == 12);
            }
            {
                auto m = mdo<maybe_t>(
                    [](){ return fjust(12); },
                    [](int v){ return fjust(v * 2); });
                REQUIRE(m.is_just());
                REQUIRE(*m == 24);
            }
            {
                auto m = mdo<maybe_t>(
                    [](){ return fjust(12); },
                    [](int v){ return fjust(v * 2); },
                    [](int v, int v2){ (void)v; return fjust(v2 * 3); });
                REQUIRE(m.is_just());
                REQUIRE(*m == 72);
            }
        }
        {
            {
                auto rr = mdo<maybe_t>(
                    fjust(42));
                REQUIRE((rr == fjust(42)));
            }
            {
                auto rr = mdo<maybe_t>(
                    fjust(12),
                    fjust(42));
                REQUIRE((rr == fjust(42)));
            }
            {
                auto rr = mdo<maybe_t>(
                    fjust(12),
                    [](){ return fjust(42); });
                REQUIRE((rr == fjust(42)));
            }
            {
                auto rr = mdo<maybe_t>(
                    [](){ return fjust(12); },
                    fjust(42));
                REQUIRE((rr == fjust(42)));
            }
            {
                auto rr = mdo<maybe_t>(
                    []()               { return fjust(32); },
                    [](int x1)         { (void)x1; return fjust(10); },
                    [](int x1, int x2) { return mreturn<maybe_t>(x1 + x2); }
                );
                REQUIRE((rr == fjust(42)));
            }
        }
    }
}
