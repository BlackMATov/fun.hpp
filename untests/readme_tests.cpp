/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/types/all.hpp"
#include "fun/types/any.hpp"

#include "fun/types/sum.hpp"
#include "fun/types/product.hpp"

#include "fun/types/box.hpp"
#include "fun/types/maybe.hpp"

#include <fun/classes/eq.hpp>
#include <fun/classes/num.hpp>
#include <fun/classes/monad.hpp>
#include <fun/classes/monoid.hpp>
#include <fun/classes/functor.hpp>
#include <fun/classes/foldable.hpp>
#include <fun/classes/applicative.hpp>
#include <fun/classes/alternative.hpp>

#include <fun/instances/eq.hpp>
#include <fun/instances/num.hpp>
#include <fun/instances/monad.hpp>
#include <fun/instances/monoid.hpp>
#include <fun/instances/functor.hpp>
#include <fun/instances/foldable.hpp>
#include <fun/instances/applicative.hpp>
#include <fun/instances/alternative.hpp>

TEST_CASE("readme_types") {
    SECTION("any/all") {
        using namespace fun;

        any_t a = make_any(true);
        REQUIRE(a.get_any());

        all_t b = make_all(false);
        REQUIRE_FALSE(b.get_all());
    }
    SECTION("sum/product") {
        using namespace fun;

        sum_t<int> a = make_sum(10);
        REQUIRE(a.get_sum() == 10);

        product_t<int> b = make_product(20);
        REQUIRE(b.get_product() == 20);
    }
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
        using namespace fun::functor_ops;

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
        using namespace fun::applicative_ops;

        maybe_t<int> m = applicative_f::apply(make_just(_+20), make_just(22));
        REQUIRE(*m == 42);

        maybe_t<int> n = make_nothing<int>();
        maybe_t<int> n1 = make_just(_+20) >>= n; // apply operator syntax
        REQUIRE(n1.is_nothing());
    }
    SECTION("alternative") {
        using namespace fun;
        using namespace fun::alternative_ops;

        maybe_t<int> m = alternative_f::alter(make_nothing<int>(), make_just<int>(42));
        REQUIRE(*m == 42);

        maybe_t<int> n = alternative_f::empty<maybe_t, int>();
        maybe_t<int> n1 = n || make_just(42); // alter operator syntax
        REQUIRE(*n1 == 42);
    }
    SECTION("monoid") {
        using namespace fun;
        using namespace fun::monoid_ops;

        REQUIRE(monoid_f::append(make_any(true), make_any(false)).get_any());
        REQUIRE(monoid_f::append(make_all(true), make_all(true)).get_all());
        REQUIRE_FALSE(monoid_f::append(make_all(true), make_all(false)).get_all());

        // and for maybe_t of monoid of course
        REQUIRE((
            make_just(make_sum(32)) >>=
            make_nothing<sum_t<int>>() >>=
            make_just(make_sum(10))
        )->get_sum() == 42);
    }
    SECTION("foldable") {
        using namespace fun;
        using namespace fun::eq_ops;
        using namespace fun::underscore;

        REQUIRE((foldable_f::fold(fjust(make_sum(10))) == fsum(10)));
        REQUIRE((foldable_f::fold(fnothing<sum_t<int>>) == fsum(0)));

        REQUIRE((foldable_f::fold_map(fsum * (_*2), fjust(10)) == fsum(20)));
        REQUIRE((foldable_f::fold_map(fsum * (_*2), fnothing<int>) == fsum(0)));
    }
    SECTION("monad") {
        using namespace fun;
        using namespace fun::eq_ops;
        using namespace fun::monad_ops;
        using namespace fun::underscore;
        {
            REQUIRE((monad_f::mreturn<maybe_t>(10) == make_just(10)));
            REQUIRE((monad_f::mbind(fjust(10), fjust * (_*2)) == fjust(20)));
            REQUIRE(((fjust(10) >>= fjust * (_*2)) == fjust(20)));
        }
        {
            auto m = monad_f::mdo<maybe_t>(
                []()               { return fjust(32); },
                [](int x1)         { (void)x1; return fjust(10); },
                [](int x1, int x2) { return monad_f::mreturn<maybe_t>(x1 + x2); }
            );
            REQUIRE((m == make_just(42)));

            auto m2 = monad_f::mdo<maybe_t>(
                fjust(32),
                fnothing<int>,
                []() { return monad_f::mreturn<maybe_t>(42); }
            );
            REQUIRE((m2 == make_nothing<int>()));
        }
    }
}
