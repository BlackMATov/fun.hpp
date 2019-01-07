/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/classes/num.hpp"
#include "fun/instances/num.hpp"

using namespace fun;
using namespace fun::underscore;

//
// user type
//

namespace
{
    template < typename A >
    struct val {
        A x, y;
        val() : x(100), y(101) {}
        val(A v) : x(v), y(v) {}
        val(A x, A y) : x(x), y(y) {}
    };
    using vali = val<int>;
}

//
// user instance
//

namespace fun {
    template < typename A >
    struct num_inst_t<val<A>, val<A>> : num_inst_t<val<A>> {
        static constexpr bool instance = true;

        static val<A> abs(const val<A>& a) {
            return val<A>(num_f::abs(a.x), num_f::abs(a.y));
        }

        static val<A> negate(const val<A>& a) {
            return val<A>(num_f::negate(a.x), num_f::negate(a.y));
        }

        static val<A> plus(const val<A>& lhs, const val<A>& rhs) {
            return val<A>(num_f::plus(lhs.x, rhs.x), num_f::plus(lhs.y, rhs.y));
        }

        static val<A> minus(const val<A>& lhs, const val<A>& rhs) {
            return val<A>(num_f::minus(lhs.x, rhs.x), num_f::minus(lhs.y, rhs.y));
        }

        static val<A> multiplies(const val<A>& lhs, const val<A>& rhs) {
            return val<A>(num_f::multiplies(lhs.x, rhs.x), num_f::multiplies(lhs.y, rhs.y));
        }
    };
}

TEST_CASE("num"){
    SECTION("plus_minus_multiplies") {
        REQUIRE(num_f::plus(2,3) == 5);
        REQUIRE(num_f::minus(2,3) == -1);
        REQUIRE(num_f::multiplies(2,3) == 6);

        REQUIRE(num_f::plus(vali(2),vali(3)).x == 5);
        REQUIRE(num_f::minus(vali(2),vali(3)).x == -1);
        REQUIRE(num_f::multiplies(vali(2),vali(3)).x == 6);

        REQUIRE(*num_f::plus(make_box(10),make_box(22)) == 32);
        REQUIRE(num_f::plus(make_sum(10),make_sum(22)).get_sum() == 32);
        REQUIRE(num_f::plus(make_product(10),make_product(22)).get_product() == 32);

        REQUIRE(*num_f::minus(make_box(10),make_box(22)) == -12);
        REQUIRE(num_f::minus(make_sum(10),make_sum(22)).get_sum() == -12);
        REQUIRE(num_f::minus(make_product(10),make_product(22)).get_product() == -12);

        REQUIRE(*num_f::multiplies(make_box(10),make_box(22)) == 220);
        REQUIRE(num_f::multiplies(make_sum(10),make_sum(22)).get_sum() == 220);
        REQUIRE(num_f::multiplies(make_product(10),make_product(22)).get_product() == 220);
    }
    SECTION("abs_negate") {
        REQUIRE(num_f::abs( 2) == 2);
        REQUIRE(num_f::abs(-2) == 2);
        REQUIRE(num_f::abs(vali(-2)).x == 2);

        REQUIRE(num_f::abs( 2.f) == 2.f);
        REQUIRE(num_f::abs(-2.f) == 2.f);
        REQUIRE(num_f::abs(vali(-2.f)).x == 2.f);

        REQUIRE(num_f::abs(make_sum(-10)).get_sum() == 10);
        REQUIRE(num_f::abs(make_product(-10)).get_product() == 10);
        REQUIRE(*num_f::abs(make_box(-10)) == 10);

        REQUIRE(num_f::negate(2) == -2);
        REQUIRE(num_f::negate(-2) == 2);
        REQUIRE(num_f::negate(vali(2)).x == -2);

        REQUIRE(num_f::negate(make_sum(10)).get_sum() == -10);
        REQUIRE(num_f::negate(make_product(10)).get_product() == -10);
        REQUIRE(*num_f::negate(make_box(10)) == -10);
    }
    SECTION("operators") {
        REQUIRE((-vali(2)).x == -2);
        REQUIRE((vali(2) + vali(3)).x == 5);
        REQUIRE((vali(2) - vali(3)).x == -1);
        REQUIRE((vali(2) * vali(3)).x == 6);

        REQUIRE(*(make_box(10) + make_box(20)) == 30);
        REQUIRE((make_sum(2) + make_sum(4)).get_sum() == 6);
        REQUIRE((make_product(2) + make_product(4)).get_product() == 6);
    }
}
