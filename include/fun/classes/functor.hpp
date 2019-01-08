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
    // functor_inst_t
    //

    template
    <
        template <typename...> class T,
        template <typename...> class = std::void_t
    >
    struct functor_inst_t : template_type_instance_t<functor_inst_t> {
        // ---------------------------------------------------------------------
        // Minimal for T<A>
        //
        // template < typename A
        //          , typename F
        //          , typename B = std::invoke_result_t<F,A> >
        // static T<B> fmap(F f, const T<A>& l);
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        // Minimal for T<A,B>
        //
        // template < typename A
        //          , typename B
        //          , typename F
        //          , typename C = std::invoke_result_t<F,B> >
        // static T<A,C> fmap(F f, const T<A,B>& l);
        // ---------------------------------------------------------------------
    };

    //
    // functor_t
    //

    using functor_t = template_type_class_t<functor_inst_t>;

    //
    // functor_f
    //

    namespace functor_f
    {
        //
        // fmap
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As
        >
        auto fmap(F f, const T<As...>& t) {
            functor_t::check_instance<T>();
            return functor_t::instance_type<T>::fmap(f, t);
        }

        struct fmap_f {
            template < typename F, template <typename...> class T, typename... As >
            auto operator()(F f, const T<As...>& t) const {
                return fmap(f, t);
            }
        };
        inline const auto ffmap = curry(fmap_f());

        //
        // ffmap_const
        //

        template
        <
            typename B,
            template <typename...> class T,
            typename... As
        >
        auto fmap_const(const B& b, const T<As...>& t) {
            functor_t::check_instance<T>();
            return functor_t::instance_type<T>::fmap(fconst(b), t);
        }

        struct fmap_const_f {
            template < typename B, template <typename...> class T, typename... As >
            auto operator()(const B& b, const T<As...>& t) const {
                return fmap_const(b, t);
            }
        };
        inline const auto ffmap_const = curry(fmap_const_f());
    }

    //
    // functor operators
    //

    namespace functor_ops
    {
        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename = std::enable_if_t<functor_t::instance<T>>
        >
        auto operator>>=(F f, const T<As...>& t) {
            return functor_f::fmap(f, t);
        }

        template
        <
            typename B,
            template <typename...> class T,
            typename... As,
            typename = std::enable_if_t<functor_t::instance<T>>
        >
        auto operator>>(const B& b, const T<As...>& t) {
            return functor_f::fmap_const(b, t);
        }
    }
}
