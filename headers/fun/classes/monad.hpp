/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

#include "../classes/functor.hpp"
#include "../classes/applicative.hpp"

namespace fun
{
    //
    // monad_inst_t
    //

    template
    <
        template <typename...> class T,
        template <typename...> class = std::void_t
    >
    struct monad_inst_t : template_type_instance_t<monad_inst_t> {
        // ---------------------------------------------------------------------
        // -= Minimal for T<A> =-
        //
        // template < typename A
        //          , typename F
        //          , typename B = typename std::invoke_result_t<F,A>::value_type >
        // static T<B> mbind(const T<A>& a, F&& f);
        // ---------------------------------------------------------------------
    };

    //
    // monad_t
    //

    using monad_t = template_type_class_t<monad_inst_t>;

    //
    // monad_f
    //

    namespace monad_f
    {
        //
        // mbind decl
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As
        >
        auto mbind(const T<As...>& t, F&& f);

        //
        // mbind_const decl
        //

        template
        <
            template <typename...> class T,
            typename... As,
            typename... Bs
        >
        auto mbind_const(const T<As...>& a, const T<Bs...>& b);

        namespace mdo_detail
        {
            //
            // mdo_impl decl
            //

            template
            <
                template <typename...> class T,
                typename TArgs,
                typename... As
            >
            auto mdo_impl(TArgs&& args, const T<As...>& v);

            template
            <
                template <typename...> class T,
                typename TArgs,
                typename... As,
                typename... Fs
            >
            auto mdo_impl(TArgs&& args, const T<As...>& v, Fs&&... fs);

            template
            <
                template <typename...> class T,
                typename... Args,
                typename F,
                typename = std::enable_if_t<std::is_invocable_v<F, Args...>>
            >
            auto mdo_impl(std::tuple<Args...>&& args, F&& f);

            template
            <
                template <typename...> class T,
                typename... Args,
                typename F,
                typename... Fs,
                typename = std::enable_if_t<std::is_invocable_v<F, Args...>>
            >
            auto mdo_impl(std::tuple<Args...>&& args, F&& f, Fs&&... fs);

            //
            // mdo_impl impl
            //

            template
            <
                template <typename...> class T,
                typename TArgs,
                typename... As
            >
            auto mdo_impl(TArgs&& args, const T<As...>& v) {
                (void)args;
                return v;
            }

            template
            <
                template <typename...> class T,
                typename TArgs,
                typename... As,
                typename... Fs
            >
            auto mdo_impl(TArgs&& args, const T<As...>& v, Fs&&... fs) {
                return mbind_const(
                    v,
                    mdo_impl<T>(std::forward<TArgs>(args), std::forward<Fs>(fs)...));
            }

            template
            <
                template <typename...> class T,
                typename... Args,
                typename F,
                typename
            >
            auto mdo_impl(std::tuple<Args...>&& args, F&& f) {
                return std::apply(std::forward<F>(f), std::move(args));
            }

            template
            <
                template <typename...> class T,
                typename... Args,
                typename F,
                typename... Fs,
                typename
            >
            auto mdo_impl(std::tuple<Args...>&& args, F&& f, Fs&&... fs) {
                auto m = std::apply(std::forward<F>(f), args);
                return mbind(
                    std::move(m), [
                        args = std::move(args),
                        fs...
                    ](auto&& v){
                        auto nargs = std::tuple_cat(
                            std::move(args),
                            std::make_tuple(std::forward<decltype(v)>(v)));
                        return mdo_impl<T>(std::move(nargs), fs...);
                    });
            }
        }

        template
        <
            template <typename...> class T,
            typename... Fs
        >
        auto mdo(Fs&&... fs) {
            monad_t::check_instance<T>();
            return mdo_detail::mdo_impl<T>(
                std::make_tuple(),
                std::forward<Fs>(fs)...);
        }

        //
        // mreturn
        //

        template
        <
            template <typename...> class T,
            typename... As,
            typename A
        >
        auto mreturn(const A& a) {
            monad_t::check_instance<T>();
            return applicative_f::pure<T, As...>(a);
        }

        template < template <typename...> class T, typename... As >
        struct mreturn_f {
            template < typename A >
            auto operator()(A&& a) const {
                return mreturn<T, As...>(std::forward<A>(a));
            }
        };
        template < template <typename...> class T, typename... As >
        inline const auto fmreturn = curry(mreturn_f<T, As...>());

        //
        // mbind
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As
        >
        auto mbind(const T<As...>& t, F&& f) {
            monad_t::check_instance<T>();
            return monad_t::instance_type<T>::mbind(t, std::forward<F>(f));
        }

        struct mbind_f {
            template < typename F, template <typename...> class T, typename... As >
            auto operator()(const T<As...>& t, F&& f) const {
                return mbind(t, std::forward<F>(f));
            }
        };
        inline const auto fmbind = curry(mbind_f());

        //
        // mbind_const
        //

        template
        <
            template <typename...> class T,
            typename... As,
            typename... Bs
        >
        auto mbind_const(const T<As...>& a, const T<Bs...>& b) {
            monad_t::check_instance<T>();
            return mbind(a, fconst(b));
        }

        struct mbind_const_f {
            template < template <typename...> class T, typename... As, typename... Bs >
            auto operator()(const T<As...>& a, const T<Bs...>& b) const {
                return mbind_const(a, b);
            }
        };
        inline const auto fmbind_const = curry(mbind_const_f());

        //
        // lift_m
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As
        >
        auto lift_m(F&& f, const T<As...>& a) {
            monad_t::check_instance<T>();
            return mdo<T>(
                [&a]() { return a; },
                [f = std::forward<F>(f)](auto&& av) {
                    return mreturn<T>(f(
                        std::forward<decltype(av)>(av))); });
        }

        struct lift_mf {
            template < typename F, template <typename...> class T, typename... As >
            auto operator()(F&& f, const T<As...>& a) const {
                return lift_m(std::forward<F>(f), a);
            }
        };
        inline const auto flift_m = curry(lift_mf());

        //
        // lift_m2
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename... Bs
        >
        auto lift_m2(F&& f, const T<As...>& a, const T<Bs...>& b) {
            monad_t::check_instance<T>();
            return mdo<T>(
                [&a]() { return a; },
                [&b](auto&& av) { (void)av; return b; },
                [f = std::forward<F>(f)](auto&& av, auto&& bv) {
                    return mreturn<T>(f(
                        std::forward<decltype(av)>(av),
                        std::forward<decltype(bv)>(bv))); });
        }

        struct lift_m2f {
            template < typename F, template <typename...> class T, typename... As, typename... Bs >
            auto operator()(F&& f, const T<As...>& a, const T<Bs...>& b) const {
                return lift_m2(std::forward<F>(f), a, b);
            }
        };
        inline const auto flift_m2 = curry(lift_m2f());

        //
        // lift_m3
        //

        template
        <
            typename F,
            template <typename...> class T,
            typename... As,
            typename... Bs,
            typename... Cs
        >
        auto lift_m3(F&& f, const T<As...>& a, const T<Bs...>& b, const T<Cs...>& c) {
            monad_t::check_instance<T>();
            return mdo<T>(
                [&a]() { return a; },
                [&b](auto&& av) { (void)av; return b; },
                [&c](auto&& av, auto&& bv) { (void)av; (void)bv; return c; },
                [f = std::forward<F>(f)](auto&& av, auto&& bv, auto&& cv) {
                    return mreturn<T>(f(
                        std::forward<decltype(av)>(av),
                        std::forward<decltype(bv)>(bv),
                        std::forward<decltype(cv)>(cv))); });
        }

        struct lift_m3f {
            template < typename F, template <typename...> class T, typename... As, typename... Bs, typename... Cs >
            auto operator()(F&& f, const T<As...>& a, const T<Bs...>& b, const T<Cs...>& c) const {
                return lift_m3(std::forward<F>(f), a, b, c);
            }
        };
        inline const auto flift_m3 = curry(lift_m3f());
    }

    //
    // monad operators
    //

    namespace monad_ops
    {
        template
        <
            template <typename...> class T,
            typename... As,
            typename F,
            typename = std::enable_if_t<monad_t::instance<T>>
        >
        auto operator>>=(const T<As...>& t, F&& f) {
            return monad_f::mbind(t, std::forward<F>(f));
        }

        template
        <
            template <typename...> class T,
            typename... As,
            typename... Bs,
            typename = std::enable_if_t<monad_t::instance<T>>
        >
        auto operator>>(const T<As...>& a, const T<Bs...>& b) {
            return monad_f::mbind_const(a, b);
        }
    }
}
