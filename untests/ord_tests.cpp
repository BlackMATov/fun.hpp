/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/instances/eq.hpp"

#include "fun/classes/ord.hpp"
#include "fun/instances/ord.hpp"

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::ord_ops;
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
using valf = val<float>;

//
// user instance
//

namespace fun
{
    template < typename A >
    struct ord_inst_t<val<A>, val<A>> : ord_inst_t<val<A>> {
        static constexpr bool instance = true;
        static bool less(const val<A>& lhs, const val<A>& rhs) {
            return
                ord_f::less(lhs.x, rhs.x) ||
                (!ord_f::less(rhs.x, lhs.x) && ord_f::less(lhs.y, rhs.y));
        }
    };
}

TEST_CASE("ord"){
    SECTION("less") {
        {   // char
            REQUIRE(ord_f::less('a', 'b'));
            REQUIRE_FALSE(ord_f::less('b', 'a'));

            REQUIRE_FALSE(ord_f::greater('a', 'b'));
            REQUIRE(ord_f::greater('b', 'a'));
        }
        {   // int
            REQUIRE(ord_f::less(100, 101));
            REQUIRE_FALSE(ord_f::less(101, 100));

            REQUIRE_FALSE(ord_f::greater(100, 101));
            REQUIRE(ord_f::greater(101, 100));
        }
        {   // float
            REQUIRE(ord_f::less(2.f, 3.f));
            REQUIRE_FALSE(ord_f::less(3.f, 2.f));

            REQUIRE_FALSE(ord_f::greater(2.f, 3.f));
            REQUIRE(ord_f::greater(3.f, 2.f));
        }
        {   // val char
            REQUIRE(ord_f::less(valc('a'), valc('b')));
            REQUIRE_FALSE(ord_f::less(valc('b'), valc('a')));

            REQUIRE_FALSE(ord_f::greater(valc('a'), valc('b')));
            REQUIRE(ord_f::greater(valc('b'), valc('a')));
        }
        {   // val int
            REQUIRE(ord_f::less(vali(1,1), vali(1,2)));
            REQUIRE_FALSE(ord_f::less(vali(1,2), vali(1,1)));

            REQUIRE_FALSE(ord_f::greater(vali(1), vali(2)));
            REQUIRE(ord_f::greater(vali(2), vali(1)));
        }
        {   // val float
            REQUIRE(ord_f::less(valf(2.f), valf(3.f)));
            REQUIRE_FALSE(ord_f::less(valf(3.f), valf(2.f)));

            REQUIRE_FALSE(ord_f::greater(valf(2.f), valf(3.f)));
            REQUIRE(ord_f::greater(valf(3.f), valf(2.f)));
        }
    }
    SECTION("min_max") {
        {
            REQUIRE(ord_f::min('a', 'b') == 'a');
            REQUIRE(ord_f::min(100, 101) == 100);
            REQUIRE(ord_f::min(2.f, 4.f) == 2.f);

            REQUIRE(ord_f::min('b', 'a') == 'a');
            REQUIRE(ord_f::min(101, 100) == 100);
            REQUIRE(ord_f::min(4.f, 2.f) == 2.f);
        }
        {
            REQUIRE(ord_f::max('a', 'b') == 'b');
            REQUIRE(ord_f::max(100, 101) == 101);
            REQUIRE(ord_f::max(2.f, 4.f) == 4.f);

            REQUIRE(ord_f::max('b', 'a') == 'b');
            REQUIRE(ord_f::max(101, 100) == 101);
            REQUIRE(ord_f::max(4.f, 2.f) == 4.f);
        }
        {
            REQUIRE((ord_f::min(valc('a'), valc('b')).x == 'a'));
            REQUIRE((ord_f::min(vali(100), vali(101)).x == 100));
            REQUIRE((ord_f::min(valf(2.f), valf(4.f)).x == 2.f));

            REQUIRE((ord_f::min(valc('b'), valc('a')).x == 'a'));
            REQUIRE((ord_f::min(vali(101), vali(100)).x == 100));
            REQUIRE((ord_f::min(valf(4.f), valf(2.f)).x == 2.f));
        }
        {
            REQUIRE((ord_f::max(valc('a'), valc('b')).x == 'b'));
            REQUIRE((ord_f::max(vali(100), vali(101)).x == 101));
            REQUIRE((ord_f::max(valf(2.f), valf(4.f)).x == 4.f));

            REQUIRE((ord_f::max(valc('b'), valc('a')).x == 'b'));
            REQUIRE((ord_f::max(vali(101), vali(100)).x == 101));
            REQUIRE((ord_f::max(valf(4.f), valf(2.f)).x == 4.f));
        }
        {
            REQUIRE((ord_f::min(fbox(10), fbox(20)) == fbox(10)));
            REQUIRE((ord_f::min(fbox(20), fbox(10)) == fbox(10)));
            REQUIRE((ord_f::max(fbox(10), fbox(20)) == fbox(20)));
            REQUIRE((ord_f::max(fbox(20), fbox(10)) == fbox(20)));

            REQUIRE((ord_f::min(fjust(10), fjust(20)) == fjust(10)));
            REQUIRE((ord_f::min(fjust(20), fjust(10)) == fjust(10)));
            REQUIRE((ord_f::max(fjust(10), fjust(20)) == fjust(20)));
            REQUIRE((ord_f::max(fjust(20), fjust(10)) == fjust(20)));
        }
    }
    SECTION("operators") {
        {
            REQUIRE((valc('c') < valc('d')));
            REQUIRE((vali(2) < vali(3)));
            REQUIRE((valf(2.f) < valf(3.f)));

            REQUIRE_FALSE((valc('d') < valc('c')));
            REQUIRE_FALSE((vali(3) < vali(2)));
            REQUIRE_FALSE((valf(3.f) < valf(2.f)));

            REQUIRE_FALSE((valc('c') > valc('d')));
            REQUIRE_FALSE((vali(2) > vali(3)));
            REQUIRE_FALSE((valf(2.f) > valf(3.f)));

            REQUIRE((valc('d') > valc('c')));
            REQUIRE((vali(3) > vali(2)));
            REQUIRE((valf(3.f) > valf(2.f)));
        }
        {
            REQUIRE((make_all(false) < make_all(true)));
            REQUIRE((make_all(false) <= make_all(true)));
            REQUIRE((make_all(true) <= make_all(true)));
            REQUIRE((make_all(true) > make_all(false)));
            REQUIRE((make_all(true) >= make_all(false)));
            REQUIRE((make_all(true) >= make_all(true)));

            REQUIRE_FALSE((make_all(false) > make_all(true)));
            REQUIRE_FALSE((make_all(false) > make_all(true)));
            REQUIRE_FALSE((make_all(true) > make_all(true)));
            REQUIRE_FALSE((make_all(true) < make_all(false)));
            REQUIRE_FALSE((make_all(true) < make_all(false)));
            REQUIRE_FALSE((make_all(true) < make_all(true)));

            REQUIRE((make_any(false) < make_any(true)));
            REQUIRE((make_any(false) <= make_any(true)));
            REQUIRE((make_any(true) <= make_any(true)));
            REQUIRE((make_any(true) > make_any(false)));
            REQUIRE((make_any(true) >= make_any(false)));
            REQUIRE((make_any(true) >= make_any(true)));

            REQUIRE_FALSE((make_any(false) > make_any(true)));
            REQUIRE_FALSE((make_any(false) > make_any(true)));
            REQUIRE_FALSE((make_any(true) > make_any(true)));
            REQUIRE_FALSE((make_any(true) < make_any(false)));
            REQUIRE_FALSE((make_any(true) < make_any(false)));
            REQUIRE_FALSE((make_any(true) < make_any(true)));

            REQUIRE((make_sum(10) < make_sum(20)));
            REQUIRE((make_sum(10) <= make_sum(20)));
            REQUIRE((make_sum(10) <= make_sum(10)));
            REQUIRE((make_sum(20) > make_sum(10)));
            REQUIRE((make_sum(20) >= make_sum(10)));
            REQUIRE((make_sum(20) >= make_sum(20)));

            REQUIRE_FALSE((make_sum(10) > make_sum(20)));
            REQUIRE_FALSE((make_sum(10) > make_sum(20)));
            REQUIRE_FALSE((make_sum(10) > make_sum(10)));
            REQUIRE_FALSE((make_sum(20) < make_sum(10)));
            REQUIRE_FALSE((make_sum(20) < make_sum(10)));
            REQUIRE_FALSE((make_sum(20) < make_sum(20)));

            REQUIRE((make_product(10) < make_product(20)));
            REQUIRE((make_product(10) <= make_product(20)));
            REQUIRE((make_product(10) <= make_product(10)));
            REQUIRE((make_product(20) > make_product(10)));
            REQUIRE((make_product(20) >= make_product(10)));
            REQUIRE((make_product(20) >= make_product(20)));

            REQUIRE_FALSE((make_product(10) > make_product(20)));
            REQUIRE_FALSE((make_product(10) > make_product(20)));
            REQUIRE_FALSE((make_product(10) > make_product(10)));
            REQUIRE_FALSE((make_product(20) < make_product(10)));
            REQUIRE_FALSE((make_product(20) < make_product(10)));
            REQUIRE_FALSE((make_product(20) < make_product(20)));

            REQUIRE((fbox(10) < fbox(20)));
            REQUIRE((fbox(10) <= fbox(20)));
            REQUIRE((fbox(10) <= fbox(10)));
            REQUIRE((fbox(20) > fbox(10)));
            REQUIRE((fbox(20) >= fbox(10)));
            REQUIRE((fbox(20) >= fbox(20)));

            REQUIRE_FALSE((fbox(10) > fbox(20)));
            REQUIRE_FALSE((fbox(10) > fbox(20)));
            REQUIRE_FALSE((fbox(10) > fbox(10)));
            REQUIRE_FALSE((fbox(20) < fbox(10)));
            REQUIRE_FALSE((fbox(20) < fbox(10)));
            REQUIRE_FALSE((fbox(20) < fbox(20)));

            REQUIRE((fjust(10) < fjust(20)));
            REQUIRE((fjust(10) <= fjust(20)));
            REQUIRE((fjust(10) <= fjust(10)));
            REQUIRE((fjust(20) > fjust(10)));
            REQUIRE((fjust(20) >= fjust(10)));
            REQUIRE((fjust(20) >= fjust(20)));

            REQUIRE_FALSE((fjust(10) > fjust(20)));
            REQUIRE_FALSE((fjust(10) > fjust(20)));
            REQUIRE_FALSE((fjust(10) > fjust(10)));
            REQUIRE_FALSE((fjust(20) < fjust(10)));
            REQUIRE_FALSE((fjust(20) < fjust(10)));
            REQUIRE_FALSE((fjust(20) < fjust(20)));

            REQUIRE((fnothing<int> < fjust(0)));
            REQUIRE((fnothing<int> <= fjust(0)));
            REQUIRE_FALSE((fnothing<int> > fjust(0)));
            REQUIRE_FALSE((fnothing<int> > fjust(0)));

            REQUIRE((fjust(0) > fnothing<int>));
            REQUIRE((fjust(0) >= fnothing<int>));
            REQUIRE_FALSE((fjust(0) < fnothing<int>));
            REQUIRE_FALSE((fjust(0) < fnothing<int>));

            REQUIRE((fjust(vali(10)) < fjust(vali(20))));
            REQUIRE_FALSE((fjust(vali(10)) > fjust(vali(20))));
        }
    }
}
