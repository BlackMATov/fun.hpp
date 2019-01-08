/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

namespace fun
{
    //
    // eq_inst_t
    //

    template < typename A, typename = void >
    struct eq_inst_t : type_instance_t<eq_inst_t> {
        static bool equals(const A& l, const A& r) {
            return !instance_type<A>::not_equals(l, r);
        }

        static bool not_equals(const A& l, const A& r) {
            return !instance_type<A>::equals(l, r);
        }
    };

    //
    // eq_t
    //

    using eq_t = type_class_t<eq_inst_t>;

    //
    // eq_f
    //

    namespace eq_f
    {
        template < typename A >
        bool equals(const A& l, const A& r) {
            eq_t::check_instance<A>();
            return eq_t::instance_type<A>::equals(l, r);
        }

        template < typename A >
        bool not_equals(const A& l, const A& r) {
            eq_t::check_instance<A>();
            return eq_t::instance_type<A>::not_equals(l, r);
        }
    }

    //
    // eq operators
    //

    namespace eq_ops
    {
        template < typename A, typename = std::enable_if_t<eq_t::instance<A>> >
        bool operator==(const A& lhs, const A& rhs) {
            return eq_f::equals(lhs, rhs);
        }

        template < typename A, typename = std::enable_if_t<eq_t::instance<A>> >
        bool operator!=(const A& lhs, const A& rhs) {
            return eq_f::not_equals(lhs, rhs);
        }
    }
}
