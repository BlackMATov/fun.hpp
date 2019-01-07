/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/classes/functor.hpp"
#include "fun/instances/functor.hpp"

using namespace fun;
using namespace fun::underscore;

TEST_CASE("functor"){
    {
        REQUIRE(functor_f::fmap(_*2, make_sum(10)).get_sum() == 20);
        REQUIRE(functor_f::fmap(_*2, make_product(20)).get_product() == 40);
    }
    {
        auto m = make_just(42);
        auto m1 = functor_f::fmap(_ * 2, m);
        auto m2 = functor_f::ffmap(_ * 2, m);
        REQUIRE(*m == 42);
        REQUIRE(*m1 == 84);
        REQUIRE(*m2 == 84);
    }
    {
        auto m = make_just(42);
        auto m1 = functor_f::fmap_const(84, m);
        auto m2 = functor_f::ffmap_const(84, m);
        REQUIRE(*m == 42);
        REQUIRE(*m1 == 84);
        REQUIRE(*m2 == 84);
    }
    SECTION("operators") {
        using namespace fun::functor_ops;
        {
            const auto m0 = make_just(21);
            const auto m1 = (_ * 2) >>= m0;
            REQUIRE(*m1 == 42);
        }
        {
            auto b = make_box(42);
            auto b1 = _ * 2 >>= b;
            REQUIRE(*b1 == 84);
        }
        {
            auto b = make_box(42);
            auto b1 = 84 >> b;
            REQUIRE(*b1 == 84);
        }
    }
}
