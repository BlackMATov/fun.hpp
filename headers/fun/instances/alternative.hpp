/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_instances.hpp"

#include "../types/maybe.hpp"

#include "../classes/applicative.hpp"
#include "../instances/applicative.hpp"

#include "../classes/alternative.hpp"

namespace fun
{
    template <>
    struct alternative_inst_t<maybe_t, maybe_t> : alternative_inst_t<maybe_t> {
        static constexpr bool instance = true;

        template < typename A >
        static maybe_t<A> empty() {
            return make_nothing<A>();
        }

        template < typename A >
        static maybe_t<A> alter(const maybe_t<A>& lhs, const maybe_t<A>& rhs) {
            return lhs.is_just() ? lhs : rhs;
        }
    };
}
