/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

#include "../classes/functor.hpp"

namespace fun
{
    //
    // applicative_inst_t
    //

    template
    <
        template <typename...> class T,
        template <typename...> class = std::void_t
    >
    struct applicative_inst_t : template_type_instance_t<applicative_inst_t> {
        // ---------------------------------------------------------------------
        // -= Minimal for T<A> =-
        //
        // template < typename A >
        // static T<A> pure(const A&);
        //
        // template < typename A
        //          , typename F
        //          , typename B = std::invoke_result_t<F,A> >
        // static T<B> apply(const T<F>& mf, const T<A>& ma);
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        // -= Minimal for T<A,B> =-
        //
        // template < typename A, typename B >
        // static T<A,B> pure(const B& b);
        //
        // template < typename A
        //          , typename B
        //          , typename F
        //          , typename C = std::invoke_result_t<F,B> >
        // static T<A,C> apply(const T<A,F>& f, const T<A,B>& t);
        // ---------------------------------------------------------------------
    };

    //
    // applicative_t
    //

    using applicative_t = template_type_class_t<applicative_inst_t>;

    //
    // applicative_f
    //

    namespace applicative_f
    {
        //
        // pure
        //

        template
        <
            template <typename...> class T,
            typename... As,
            typename A
        >
        auto pure(const A& a) {
            applicative_t::check_instance<T>();
            return applicative_t::instance_type<T>::template pure<As...>(a);
        }

        template < template <typename...> class T, typename... As >
        struct pure_f {
            template < typename A >
            auto operator()(const A& a) const {
                return pure<T,As...>(a);
            }
        };
        template < template <typename...> class T, typename... As >
        inline const auto fpure = curry(pure_f<T,As...>());

        //
        // apply
        //

        template
        <
            template <typename...> class T,
            typename... Fs,
            typename... As
        >
        auto apply(const T<Fs...>& f, const T<As...>& a) {
            applicative_t::check_instance<T>();
            return applicative_t::instance_type<T>::apply(f, a);
        }

        struct apply_f {
            template < template <typename...> class T, typename... Fs, typename... As >
            auto operator()(const T<Fs...>& f, const T<As...>& a) const {
                return apply(f, a);
            }
        };
        inline const auto fapply = curry(apply_f());

        //
        // lift_a2
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename... Bs
        >
        auto lift_a2(F f, const T<As...>& a, const T<Bs...>& b) {
            applicative_t::check_instance<T>();
            return applicative_f::apply(functor_f::fmap(curry(f), a), b);
        }

        struct lift_a2_f {
            template < typename F, template <typename...> class T, typename... As, typename... Bs >
            auto operator()(F f, const T<As...>& a, const T<Bs...>& b) const {
                return lift_a2(f, a, b);
            }
        };
        inline const auto flift_a2 = curry(lift_a2_f());

        //
        // lift_a3
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename... Bs,
            typename... Cs
        >
        auto lift_a3(F f, const T<As...>& a, const T<Bs...>& b, const T<Cs...>& c) {
            applicative_t::check_instance<T>();
            return applicative_f::apply(lift_a2(curry(f), a, b), c);
        }

        struct lift_a3_f {
            template < typename F, template <typename...> class T, typename... As, typename... Bs, typename... Cs >
            auto operator()(F f, const T<As...>& a, const T<Bs...>& b, const T<Cs...>& c) const {
                return lift_a3(f, a, b, c);
            }
        };
        inline const auto flift_a3 = curry(lift_a3_f());
    }

    //
    // applicative operators
    //

    namespace applicative_ops
    {
        template
        <
            template <typename...> class T,
            typename... Fs,
            typename... As,
            typename = std::enable_if_t<applicative_t::instance<T>>
        >
        auto operator>>=(const T<Fs...>& f, const T<As...>& a) {
            return applicative_f::apply(f, a);
        }
    }
}
