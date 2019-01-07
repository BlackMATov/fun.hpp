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
    // monoid_inst_t
    //

    template < typename A, typename = void >
    struct monoid_inst_t : type_instance_t<monoid_inst_t> {
        // ---------------------------------------------------------------------
        // -= Minimal for A =-
        // static A empty();
        // static A append(const A&, const A&);
        // ---------------------------------------------------------------------
    };

    //
    // monoid_t
    //

    using monoid_t = type_class_t<monoid_inst_t>;

    //
    // monoid_f
    //

    namespace monoid_f
    {
        template < typename A >
        A empty() {
            monoid_t::check_instance<A>();
            return monoid_t::instance_type<A>::empty();
        }

        template < typename A >
        A append(const A& lhs, const A& rhs) {
            monoid_t::check_instance<A>();
            return monoid_t::instance_type<A>::append(lhs, rhs);
        }

        //
        //
        //

        template < typename A >
        struct empty_f {
            A operator()() const {
                return empty<A>();
            }
        };
        template < typename A >
        inline const auto fempty = empty_f<A>()();

        struct append_f {
            template < typename A >
            A operator()(const A& lhs, const A& rhs) const {
                return append(lhs, rhs);
            }
        };
        inline const auto fappend = curry(append_f());
    }
}
