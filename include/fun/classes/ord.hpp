/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

#include <type_traits>

namespace fun
{
    //
    // ord_inst_t
    //

    template < typename A, typename = void >
    struct ord_inst_t : type_instance_t<ord_inst_t> {
        static bool less(const A& lhs, const A& rhs) {
            return ordering::lt == instance_type<A>::compare(lhs, rhs);
        }

        static ordering compare(const A& lhs, const A& rhs) {
            if ( instance_type<A>::less(lhs, rhs) ) {
                return ordering::lt;
            }
            if ( instance_type<A>::less(rhs, lhs) ) {
                return ordering::gt;
            }
            return ordering::eq;
        }
    };

    //
    // ord_t
    //

    using ord_t = type_class_t<ord_inst_t>;

    //
    // ord_f
    //

    namespace ord_f
    {
        template < typename A >
        bool less(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return ord_t::instance_type<A>::less(lhs, rhs);
        }

        template < typename A >
        bool less_or_equals(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return !ord_t::instance_type<A>::less(rhs, lhs);
        }

        template < typename A >
        bool greater(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return ord_t::instance_type<A>::less(rhs, lhs);
        }

        template < typename A >
        bool greater_or_equals(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return !ord_t::instance_type<A>::less(lhs, rhs);
        }

        template < typename A >
        const A& min(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return ord_t::instance_type<A>::less(lhs, rhs) ? lhs : rhs;
        }

        template < typename A >
        const A& max(const A& lhs, const A& rhs) {
            ord_t::check_instance<A>();
            return ord_t::instance_type<A>::less(lhs, rhs) ? rhs : lhs;
        }
    }

    //
    // ord operators
    //

    namespace ord_ops
    {
        template < typename A, typename = std::enable_if_t<ord_t::instance<A>> >
        bool operator<(const A& lhs, const A& rhs) {
            return ord_f::less(lhs, rhs);
        }

        template < typename A, typename = std::enable_if_t<ord_t::instance<A>> >
        bool operator<=(const A& lhs, const A& rhs) {
            return ord_f::less_or_equals(lhs, rhs);
        }

        template < typename A, typename = std::enable_if_t<ord_t::instance<A>> >
        bool operator>(const A& lhs, const A& rhs) {
            return ord_f::greater(lhs, rhs);
        }

        template < typename A, typename = std::enable_if_t<ord_t::instance<A>> >
        bool operator>=(const A& lhs, const A& rhs) {
            return ord_f::greater_or_equals(lhs, rhs);
        }
    }
}
