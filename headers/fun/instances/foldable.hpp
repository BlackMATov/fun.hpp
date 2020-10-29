/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include "../types/sum.hpp"
#include "../types/product.hpp"

#include "../types/box.hpp"
#include "../types/maybe.hpp"

#include "../classes/monoid.hpp"
#include "../instances/monoid.hpp"

#include "../classes/foldable.hpp"

namespace fun
{
    template <>
    struct foldable_inst_t<sum_t, sum_t> : foldable_inst_t<sum_t> {
        static constexpr bool instance = true;

        template < typename A, typename B, typename F >
        static B foldr(F&& f, B&& acc, const sum_t<A>& s) {
            static_assert(
                std::is_convertible<F, std::function<B(A, B)>>::value,
                "foldr requires a function type (a -> b -> b)");
            return std::forward<F>(f)(s.get_sum(), std::forward<B>(acc));
        }
    };

    template <>
    struct foldable_inst_t<product_t, product_t> : foldable_inst_t<product_t> {
        static constexpr bool instance = true;

        template < typename A, typename B, typename F >
        static B foldr(F&& f, B&& acc, const product_t<A>& p) {
            static_assert(
                std::is_convertible<F, std::function<B(A, B)>>::value,
                "foldr requires a function type (a -> b -> b)");
            return std::forward<F>(f)(p.get_product(), std::forward<B>(acc));
        }
    };

    template <>
    struct foldable_inst_t<box_t, box_t> : foldable_inst_t<box_t> {
        static constexpr bool instance = true;

        template < typename A, typename B, typename F >
        static B foldr(F&& f, B&& acc, const box_t<A>& b) {
            static_assert(
                std::is_convertible<F, std::function<B(A, B)>>::value,
                "foldr requires a function type (a -> b -> b)");
            return std::forward<F>(f)(*b, std::forward<B>(acc));
        }
    };

    template <>
    struct foldable_inst_t<maybe_t, maybe_t> : foldable_inst_t<maybe_t> {
        static constexpr bool instance = true;

        template < typename A, typename B, typename F >
        static B foldr(F&& f, B&& acc, const maybe_t<A>& m) {
            static_assert(
                std::is_convertible<F, std::function<B(A, B)>>::value,
                "foldr requires a function type (a -> b -> b)");
            return m.is_nothing()
                ? std::forward<B>(acc)
                : std::forward<F>(f)(*m, std::forward<B>(acc));
        }
    };
}
