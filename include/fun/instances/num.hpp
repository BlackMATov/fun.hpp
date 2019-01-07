/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include <type_traits>

#include "../types/sum.hpp"
#include "../types/product.hpp"

#include "../types/box.hpp"

#include "../classes/num.hpp"

namespace fun
{
    //
    // signed types
    //

    template < typename A >
    struct num_inst_t<A,
        std::enable_if_t<
            std::is_signed<A>::value, A>
        > : num_inst_t<A>
    {
        static constexpr bool instance = true;

        static A abs(A a) {
            return std::abs(a);
        }

        static A negate(A a) {
            return -a;
        }

        static A plus(A l, A r) {
            return l + r;
        }

        static A minus(A l, A r) {
            return l - r;
        }

        static A multiplies(A l, A r) {
            return l * r;
        }
    };

    //
    // sum_t
    //

    template < typename A >
    struct num_inst_t<sum_t<A>,
        std::enable_if_t<
            num_t::instance<A>, sum_t<A>>
        > : num_inst_t<A>
    {
        static constexpr bool instance = true;

        static sum_t<A> abs(const sum_t<A>& a) {
            return make_sum(std::abs(a.get_sum()));
        }

        static sum_t<A> negate(const sum_t<A>& a) {
            return make_sum(-a.get_sum());
        }

        static sum_t<A> plus(const sum_t<A>& l, const sum_t<A>& r) {
            return make_sum(l.get_sum() + r.get_sum());
        }

        static sum_t<A> minus(const sum_t<A>& l, const sum_t<A>& r) {
            return make_sum(l.get_sum() - r.get_sum());
        }

        static sum_t<A> multiplies(const sum_t<A>& l, const sum_t<A>& r) {
            return make_sum(l.get_sum() * r.get_sum());
        }
    };

    //
    // product_t
    //

    template < typename A >
    struct num_inst_t<product_t<A>,
        std::enable_if_t<
            num_t::instance<A>, product_t<A>>
        > : num_inst_t<A>
    {
        static constexpr bool instance = true;

        static product_t<A> abs(const product_t<A>& a) {
            return make_product(std::abs(a.get_product()));
        }

        static product_t<A> negate(const product_t<A>& a) {
            return make_product(-a.get_product());
        }

        static product_t<A> plus(const product_t<A>& l, const product_t<A>& r) {
            return make_product(l.get_product() + r.get_product());
        }

        static product_t<A> minus(const product_t<A>& l, const product_t<A>& r) {
            return make_product(l.get_product() - r.get_product());
        }

        static product_t<A> multiplies(const product_t<A>& l, const product_t<A>& r) {
            return make_product(l.get_product() * r.get_product());
        }
    };

    //
    // box_t
    //

    template < typename A >
    struct num_inst_t<box_t<A>,
        std::enable_if_t<
            num_t::instance<A>, box_t<A>>
        > : num_inst_t<A>
    {
        static constexpr bool instance = true;

        static box_t<A> abs(const box_t<A>& a) {
            return make_box(std::abs(*a));
        }

        static box_t<A> negate(const box_t<A>& a) {
            return make_box(-*a);
        }

        static box_t<A> plus(const box_t<A>& l, const box_t<A>& r) {
            return make_box(*l + *r);
        }

        static box_t<A> minus(const box_t<A>& l, const box_t<A>& r) {
            return make_box(*l - *r);
        }

        static box_t<A> multiplies(const box_t<A>& l, const box_t<A>& r) {
            return make_box(*l * *r);
        }
    };
}
