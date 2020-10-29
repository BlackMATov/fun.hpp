/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include "../types/all.hpp"
#include "../types/any.hpp"
#include "../types/sum.hpp"
#include "../types/product.hpp"

#include "../types/box.hpp"
#include "../types/maybe.hpp"

#include "../classes/num.hpp"
#include "../instances/num.hpp"

#include "../classes/monoid.hpp"

namespace fun
{
    //
    // all_t
    //

    template <>
    struct monoid_inst_t<all_t, all_t> : monoid_inst_t<all_t> {
        static constexpr bool instance = true;

        static all_t empty() {
            return all_t(true);
        }

        static all_t append(const all_t& lhs, const all_t& rhs) {
            return all_t(lhs.get_all() && rhs.get_all());
        }
    };

    //
    // any_t
    //

    template <>
    struct monoid_inst_t<any_t, any_t> : monoid_inst_t<any_t> {
        static constexpr bool instance = true;

        static any_t empty() {
            return any_t(false);
        }

        static any_t append(const any_t& lhs, const any_t& rhs) {
            return any_t(lhs.get_any() || rhs.get_any());
        }
    };

    //
    // sum_t
    //

    template < typename A >
    struct monoid_inst_t<sum_t<A>,
        std::enable_if_t<
            num_t::instance<A>, sum_t<A>>
        > : monoid_inst_t<sum_t<A>>
    {
        static constexpr bool instance = true;

        static sum_t<A> empty() {
            return sum_t<A>(A(0));
        }

        static sum_t<A> append(const sum_t<A>& lhs, const sum_t<A>& rhs) {
            return sum_t<A>(num_f::plus(lhs.get_sum(), rhs.get_sum()));
        }
    };

    //
    // product_t
    //

    template < typename A >
    struct monoid_inst_t<product_t<A>,
        std::enable_if_t<
            num_t::instance<A>, product_t<A>>
        > : monoid_inst_t<product_t<A>>
    {
        static constexpr bool instance = true;

        static product_t<A> empty() {
            return product_t<A>(A(1));
        }

        static product_t<A> append(const product_t<A>& lhs, const product_t<A>& rhs) {
            return product_t<A>(num_f::multiplies(lhs.get_product(), rhs.get_product()));
        }
    };

    //
    // maybe_t
    //

    template < typename A >
    struct monoid_inst_t<maybe_t<A>,
        std::enable_if_t<
            monoid_t::instance<A>, maybe_t<A>>
        > : monoid_inst_t<maybe_t<A>>
    {
        static constexpr bool instance = true;

        static maybe_t<A> empty() {
            return make_nothing<A>();
        }

        static maybe_t<A> append(const maybe_t<A>& lhs, const maybe_t<A>& rhs) {
            if ( lhs.is_nothing() ) {
                return rhs;
            }
            if ( rhs.is_nothing() ) {
                return lhs;
            }
            return make_just(monoid_f::append(*lhs, *rhs));
        }
    };
}
