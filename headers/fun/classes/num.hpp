/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

namespace fun
{
    //
    // num_inst_t
    //

    template < typename A, typename = void >
    struct num_inst_t : type_instance_t<num_inst_t> {
        static A abs(const A& a);
        static A negate(const A& a);
        static A plus(const A& l, const A& r);
        static A minus(const A& l, const A& r);
        static A multiplies(const A& l, const A& r);
    };

    template < typename A, typename DUMMY >
    A num_inst_t<A,DUMMY>::negate(const A& a) {
        return instance_type<A>::minus(A(0), a);
    }

    template < typename A, typename DUMMY >
    A num_inst_t<A,DUMMY>::minus(const A& l, const A& r) {
        return instance_type<A>::plus(l, instance_type<A>::negate(r));
    }

    //
    // num_t
    //

    using num_t = type_class_t<num_inst_t>;

    //
    // num_f
    //

    namespace num_f
    {
        template < typename A >
        A abs(const A& a) {
            num_t::check_instance<A>();
            return num_t::instance_type<A>::abs(a);
        }

        template < typename A >
        A negate(const A& a) {
            num_t::check_instance<A>();
            return num_t::instance_type<A>::negate(a);
        }

        template < typename A >
        A plus(const A& l, const A& r) {
            num_t::check_instance<A>();
            return num_t::instance_type<A>::plus(l, r);
        }

        template < typename A >
        A minus(const A& l, const A& r) {
            num_t::check_instance<A>();
            return num_t::instance_type<A>::minus(l, r);
        }

        template < typename A >
        A multiplies(const A& l, const A& r) {
            num_t::check_instance<A>();
            return num_t::instance_type<A>::multiplies(l, r);
        }
    }

    //
    // num operators
    //

    namespace num_ops
    {
        template < typename A, typename = std::enable_if_t<num_t::instance<A>> >
        A operator-(const A& a) {
            return num_f::negate(a);
        }

        template < typename A, typename = std::enable_if_t<num_t::instance<A>> >
        A operator+(const A& l, const A& r) {
            return num_f::plus(l, r);
        }

        template < typename A, typename = std::enable_if_t<num_t::instance<A>> >
        A operator-(const A& l, const A& r) {
            return num_f::minus(l, r);
        }

        template < typename A, typename = std::enable_if_t<num_t::instance<A>> >
        A operator*(const A& l, const A& r) {
            return num_f::multiplies(l, r);
        }
    }
}
