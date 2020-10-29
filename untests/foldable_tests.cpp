/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include "fun/instances/eq.hpp"

#include "fun/classes/foldable.hpp"
#include "fun/instances/foldable.hpp"

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::underscore;

TEST_CASE("foldable"){
    SECTION("fold") {
        REQUIRE((foldable_f::fold(make_sum(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::fold(make_sum(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::fold(make_product(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::fold(make_product(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::fold(fbox(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::fold(fbox(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::fold(fjust(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::fold(fnothing<sum_t<int>>) == make_sum(0)));

        REQUIRE((foldable_f::fold(fjust(make_product(10))) == make_product(10)));
        REQUIRE((foldable_f::fold(fnothing<product_t<int>>) == make_product(1)));
    }
    SECTION("ffold") {
        REQUIRE((foldable_f::ffold(make_sum(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::ffold(make_sum(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::ffold(make_product(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::ffold(make_product(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::ffold(fbox(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::ffold(fbox(make_product(10))) == make_product(10)));

        REQUIRE((foldable_f::ffold(fjust(make_sum(10))) == make_sum(10)));
        REQUIRE((foldable_f::ffold(fnothing<sum_t<int>>) == make_sum(0)));

        REQUIRE((foldable_f::ffold(fjust(make_product(10))) == make_product(10)));
        REQUIRE((foldable_f::ffold(fnothing<product_t<int>>) == make_product(1)));
    }
    SECTION("fold_map"){
        REQUIRE((foldable_f::fold_map(fsum * (_*2), fbox(10)) == fsum(20)));
        REQUIRE((foldable_f::fold_map(fsum * (_*2), fjust(10)) == fsum(20)));
        REQUIRE((foldable_f::fold_map(fsum * (_*2), fnothing<int>) == fsum(0)));
    }
    SECTION("ffold_map"){
        REQUIRE((foldable_f::ffold_map(fproduct * (_*2), fbox(10)) == fproduct(20)));
        REQUIRE((foldable_f::ffold_map(fproduct * (_*2), fjust(10)) == fproduct(20)));
        REQUIRE((foldable_f::ffold_map(fproduct * (_*2), fnothing<int>) == fproduct(1)));
    }
}
