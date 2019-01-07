/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/instances/eq.hpp"

#include "fun/classes/monoid.hpp"
#include "fun/instances/monoid.hpp"

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::underscore;

TEST_CASE("monoid"){
    SECTION("empty"){
        REQUIRE(monoid_f::empty<all_t>().get_all());
        REQUIRE_FALSE(monoid_f::empty<any_t>().get_any());

        REQUIRE(monoid_f::empty<sum_t<int>>().get_sum() == 0);
        REQUIRE(monoid_f::empty<product_t<int>>().get_product() == 1);
    }
    SECTION("append"){
        REQUIRE(monoid_f::append(make_all(true), make_all(true)).get_all());
        REQUIRE_FALSE(monoid_f::append(make_all(true), make_all(false)).get_all());
        REQUIRE_FALSE(monoid_f::append(make_all(false), make_all(true)).get_all());
        REQUIRE_FALSE(monoid_f::append(make_all(false), make_all(false)).get_all());

        REQUIRE(monoid_f::append(make_any(true), make_any(true)).get_any());
        REQUIRE(monoid_f::append(make_any(true), make_any(false)).get_any());
        REQUIRE(monoid_f::append(make_any(false), make_any(true)).get_any());
        REQUIRE_FALSE(monoid_f::append(make_any(false), make_any(false)).get_any());

        REQUIRE(monoid_f::append(make_sum(42), monoid_f::empty<sum_t<int>>()).get_sum() == 42);
        REQUIRE(monoid_f::append(make_product(42), monoid_f::empty<product_t<int>>()).get_product() == 42);

        REQUIRE(monoid_f::append(make_sum(10), make_sum(32)).get_sum() == 42);
        REQUIRE(monoid_f::append(make_product(21), make_product(2)).get_product() == 42);

        REQUIRE(monoid_f::append(make_nothing<sum_t<int>>(), make_just(make_sum(32)))->get_sum() == 32);
        REQUIRE(monoid_f::append(make_just(make_sum(32)), make_nothing<sum_t<int>>())->get_sum() == 32);
        REQUIRE(monoid_f::append(make_just(make_sum(32)), make_just<sum_t<int>>(10))->get_sum() == 42);
    }
    SECTION("operators"){
        using namespace fun::monoid_ops;
        REQUIRE((
            (make_just(make_sum(32)) >>=
            make_just(make_sum(10)) >>=
            make_nothing<sum_t<int>>()) == make_just(make_sum(42))));
    }
}
