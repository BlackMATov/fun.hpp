/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include <functional>
#include <type_traits>

#include "../types/sum.hpp"
#include "../types/product.hpp"

#include "../types/box.hpp"
#include "../types/maybe.hpp"

#include "../classes/functor.hpp"

namespace fun
{
    template <>
    struct functor_inst_t<sum_t, sum_t> : functor_inst_t<sum_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = std::invoke_result_t<F,A> >
        static sum_t<B> fmap(F f, const sum_t<A>& s) {
            return sum_t<B>(f(s.get_sum()));
        }
    };

    template <>
    struct functor_inst_t<product_t, product_t> : functor_inst_t<product_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = std::invoke_result_t<F,A> >
        static product_t<B> fmap(F f, const product_t<A>& s) {
            return product_t<B>(f(s.get_product()));
        }
    };

    template <>
    struct functor_inst_t<box_t, box_t> : functor_inst_t<box_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = std::invoke_result_t<F,A> >
        static box_t<B> fmap(F f, const box_t<A>& b) {
            static_assert(
                std::is_convertible<F, std::function<B(A)>>::value,
                "fmap requires a function type (A -> B)");
            return make_box(f(*b));
        }
    };

    template <>
    struct functor_inst_t<maybe_t, maybe_t> : functor_inst_t<maybe_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = std::invoke_result_t<F,A> >
        static maybe_t<B> fmap(F f, const maybe_t<A>& m) {
            static_assert(
                std::is_convertible<F, std::function<B(A)>>::value,
                "fmap requires a function type (A -> B)");
            return m.is_just()
                ? make_just(f(*m))
                : make_nothing<B>();
        }
    };
}
