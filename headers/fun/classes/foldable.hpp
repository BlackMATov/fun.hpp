/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

#include "../classes/monoid.hpp"

namespace fun
{
    //
    // foldable_inst_t
    //

    template
    <
        template <typename...> class T,
        template <typename...> class = std::void_t
    >
    struct foldable_inst_t : template_type_instance_t<foldable_inst_t> {
        // ---------------------------------------------------------------------
        // Minimal for T<A>
        //
        // template < typename A, typename B, typename F >
        // static B foldr(F f, B acc, const T<A>& t);
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        // Minimal for T<A,B>
        //
        // template < typename A, typename B, typename C, typename F >
        // static C foldr(F f, C acc, const T<A,B>& t);
        // ---------------------------------------------------------------------
    };

    //
    // foldable_t
    //

    using foldable_t = template_type_class_t<foldable_inst_t>;

    //
    // foldable_f
    //

    namespace foldable_f
    {
        //
        // foldr
        //

        template
        <
            typename F,
            typename B,
            template <typename...> class T,
            typename... As
        >
        auto foldr(F&& f, B&& acc, const T<As...>& t) {
            return foldable_t::instance_type<T>::foldr(
                std::forward<F>(f),
                std::forward<B>(acc),
                t);
        }

        struct foldr_f {
            template < typename F, typename B, template <typename...> class T, typename... As >
            auto operator()(F&& f, B&& acc, const T<As...>& t) const {
                return foldr(std::forward<F>(f), std::forward<B>(acc), t);
            }
        };
        inline const auto ffoldr = curry(foldr_f());

        //
        // fold_map
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename M = std::invoke_result_t<F, As...>
        >
        auto fold_map(F&& f, const T<As...>& t) {
            return foldr(
                curry(monoid_f::append<M>) * curry(std::forward<F>(f)),
                monoid_f::empty<M>(),
                t);
        }

        struct fold_map_f {
            template < typename F, template <typename...> class T, typename... As >
            auto operator()(F&& f, const T<As...>& t) const {
                return fold_map(std::forward<F>(f), t);
            }
        };
        inline const auto ffold_map = curry(fold_map_f());

        //
        // fold
        //

        template
        <
            template <typename...> class T,
            typename... As
        >
        auto fold(const T<As...>& t) {
            return fold_map(fid, t);
        }

        struct fold_f {
            template < template <typename...> class T, typename... As >
            auto operator()(const T<As...>& t) const {
                return fold(t);
            }
        };
        inline const auto ffold = curry(fold_f());
    }
}
