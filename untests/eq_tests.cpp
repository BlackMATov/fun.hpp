/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/classes/eq.hpp"
#include "fun/instances/eq.hpp"

using namespace fun;
using namespace fun::underscore;

//
// user type
//

template < typename A >
struct val {
    A x, y;
    val() : x(100), y(101) {}
    val(A v) : x(v), y(v) {}
    val(A nx, A ny) : x(nx), y(ny) {}
};

using valc = val<char>;
using vali = val<int>;

//
// user instance
//

namespace fun
{
    template < typename A >
    struct eq_inst_t<val<A>, val<A>> : eq_inst_t<val<A>> {
        static constexpr bool instance = true;
        static bool equals(const val<A>& l, const val<A>& r) {
            return
                eq_f::equals(l.x, r.x) &&
                eq_f::equals(l.y, r.y);
        }
    };
}


TEST_CASE("eq"){
    SECTION("equals") {
        {   // char
            REQUIRE(eq_f::equals('a', 'a'));
            REQUIRE_FALSE(eq_f::equals('a', 'b'));

            REQUIRE_FALSE(eq_f::not_equals('a', 'a'));
            REQUIRE(eq_f::not_equals('a', 'b'));
        }
        {   // int
            REQUIRE(eq_f::equals(100, 100));
            REQUIRE_FALSE(eq_f::equals(100, 101));

            REQUIRE_FALSE(eq_f::not_equals(100, 100));
            REQUIRE(eq_f::not_equals(100, 101));
        }
        {   // float
            REQUIRE(eq_f::equals(2.f, 2.f));
            REQUIRE_FALSE(eq_f::equals(2.f, 2.2f));

            REQUIRE_FALSE(eq_f::not_equals(2.f, 2.f));
            REQUIRE(eq_f::not_equals(2.f, 2.2f));
        }
        {   // val char
            REQUIRE(eq_f::equals(valc('a'), valc('a')));
            REQUIRE_FALSE(eq_f::equals(valc('a'), valc('b')));

            REQUIRE_FALSE(eq_f::not_equals(valc('a'), valc('a')));
            REQUIRE(eq_f::not_equals(valc('a'), valc('b')));
        }
        {   // val int
            REQUIRE(eq_f::equals(vali(1), vali(1)));
            REQUIRE_FALSE(eq_f::equals(vali(1), vali(2)));

            REQUIRE_FALSE(eq_f::not_equals(vali(1), vali(1)));
            REQUIRE(eq_f::not_equals(vali(1), vali(2)));
        }
    }
    SECTION("operators") {
        {
            REQUIRE((valc('c') == valc('c')));
            REQUIRE((vali(2) == vali(2)));

            REQUIRE_FALSE((valc('c') == valc('d')));
            REQUIRE_FALSE((vali(2) == vali(3)));

            REQUIRE_FALSE((valc('c') != valc('c')));
            REQUIRE_FALSE((vali(2) != vali(2)));

            REQUIRE((valc('c') != valc('d')));
            REQUIRE((vali(2) != vali(3)));
        }
        {
            REQUIRE(make_all(true) == make_all(true));
            REQUIRE_FALSE(make_all(true) != make_all(true));
            REQUIRE_FALSE(make_all(true) == make_all(false));
            REQUIRE(make_all(true) != make_all(false));

            REQUIRE(make_any(true) == make_any(true));
            REQUIRE_FALSE(make_any(true) != make_any(true));
            REQUIRE_FALSE(make_any(true) == make_any(false));
            REQUIRE(make_any(true) != make_any(false));

            REQUIRE(make_sum(10) == make_sum(10));
            REQUIRE_FALSE(make_sum(10) != make_sum(10));
            REQUIRE_FALSE(make_sum(10) == make_sum(11));
            REQUIRE(make_sum(10) != make_sum(11));

            REQUIRE(make_product(10) == make_product(10));
            REQUIRE_FALSE(make_product(10) != make_product(10));
            REQUIRE_FALSE(make_product(11) == make_product(10));
            REQUIRE(make_product(11) != make_product(10));

            REQUIRE(make_box(10) == make_box(10));
            REQUIRE_FALSE(make_box(10) != make_box(10));
            REQUIRE_FALSE(make_box(11) == make_box(10));
            REQUIRE(make_box(11) != make_box(10));

            REQUIRE(make_just(10) == make_just(10));
            REQUIRE_FALSE(make_just(10) != make_just(10));
            REQUIRE_FALSE(make_just(10) == make_just(11));
            REQUIRE(make_just(11) != make_just(10));

            REQUIRE_FALSE(make_just(10) == make_nothing<int>());
            REQUIRE_FALSE(make_nothing<int>() == make_just(10));
            REQUIRE(make_just(10) != make_nothing<int>());
            REQUIRE(make_nothing<int>() != make_just(10));
        }
    }
}
