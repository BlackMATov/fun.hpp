/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include "../types/box.hpp"
#include "../types/maybe.hpp"

#include "../classes/functor.hpp"
#include "../instances/functor.hpp"

#include "../classes/applicative.hpp"
#include "../instances/applicative.hpp"

#include "../classes/monad.hpp"

namespace fun
{
    template <>
    struct monad_inst_t<box_t, box_t> : monad_inst_t<box_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = typename std::invoke_result_t<F,A>::value_type >
        static box_t<B> mbind(const box_t<A>& a, F&& f) {
            static_assert(
                std::is_convertible<F, std::function<box_t<B>(A)>>::value,
                "mbind requires a function type (a -> m b)");
            return std::forward<F>(f)(*a);
        }
    };

    template <>
    struct monad_inst_t<maybe_t, maybe_t> : monad_inst_t<maybe_t> {
        static constexpr bool instance = true;

        template < typename A, typename F, typename B = typename std::invoke_result_t<F,A>::value_type >
        static maybe_t<B> mbind(const maybe_t<A>& a, F&& f) {
            static_assert(
                std::is_convertible<F, std::function<maybe_t<B>(A)>>::value,
                "mbind requires a function type (a -> m b)");
            return a.is_just()
                ? std::forward<F>(f)(*a)
                : make_nothing<B>();
        }
    };
}
