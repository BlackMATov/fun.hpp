/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/types/box.hpp"
#include "fun/types/maybe.hpp"

#include <fun/classes/functor.hpp>
#include <fun/classes/applicative.hpp>

#include <fun/instances/functor.hpp>
#include <fun/instances/applicative.hpp>

TEST_CASE("readme_types") {
    SECTION("box") {
        using namespace fun;
        box_t<int> b = make_box(42);
        REQUIRE(*b == 42);
    }
    SECTION("maybe") {
        using namespace fun;

        maybe_t<int> m = make_just(42);
        REQUIRE(m.is_just());
        REQUIRE(*m == 42);

        maybe_t<int> n = make_nothing<int>();
        REQUIRE(n.is_nothing());
        REQUIRE_THROWS(*n);
    }
}

TEST_CASE("readme_type_classes") {
    SECTION("functor") {
        using namespace fun;
        using namespace fun::underscore;

        maybe_t<int> m = make_just(21);
        maybe_t<unsigned> m1 = functor_f::fmap(_ * 2u, m);
        REQUIRE(*m1 == 42);

        maybe_t<int> n = make_nothing<int>();
        maybe_t<float> n1 = (_ * 2.f) >>= n; // fmap operator syntax
        REQUIRE(n1.is_nothing());
    }
    SECTION("applicative") {
        using namespace fun;
        using namespace fun::underscore;

        maybe_t<int> m = applicative_f::apply(make_just(_+20), make_just(22));
        REQUIRE(*m == 42);

        maybe_t<int> n = make_nothing<int>();
        maybe_t<int> n1 = make_just(_+20) >>= n; // apply operator syntax
        REQUIRE(n1.is_nothing());
    }
}
