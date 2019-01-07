/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.h"

#include <memory>
#include <stdexcept>
#include <type_traits>

namespace fun
{
    struct nothing_t {
        constexpr explicit nothing_t() = default;
    };

    struct just_ctor_t {
        constexpr explicit just_ctor_t() = default;
    };

    struct just_inplace_ctor_t {
        constexpr explicit just_inplace_ctor_t() = default;
    };

    template < typename T >
    class maybe_t final {
    public:
        maybe_t() = default;
        ~maybe_t() noexcept = default;

        maybe_t(const maybe_t&) = default;
        maybe_t& operator=(const maybe_t&) = default;

        maybe_t(nothing_t c) noexcept;

        explicit maybe_t(just_ctor_t c, T&& v);
        explicit maybe_t(just_ctor_t c, const T& v);

        template < typename... Us >
        explicit maybe_t(just_inplace_ctor_t c, Us&&... vs);

        bool is_just() const noexcept;
        bool is_nothing() const noexcept;

        const T& operator*() const;
        const T* operator->() const;
    private:
        std::shared_ptr<T> value_;
    };
}

namespace fun
{
    template < typename T >
    auto make_just() {
        return maybe_t<T>(just_ctor_t{}, T{});
    }

    template < typename T >
    auto make_just(T&& v) {
        using NT = std::decay_t<T>;
        return maybe_t<NT>(just_ctor_t{}, std::forward<T>(v));
    }

    template < typename T, typename... Us >
    auto make_just_inplace(Us&&... vs) {
        return maybe_t<T>(just_inplace_ctor_t{}, std::forward<Us>(vs)...);
    }

    template < typename T >
    auto make_nothing() {
        return maybe_t<T>();
    }

    inline nothing_t make_nothing() {
        return nothing_t{};
    }
}

namespace fun
{
    template < typename T >
    maybe_t<T>::maybe_t(nothing_t c) noexcept {
        (void)c;
    }

    template < typename T >
    maybe_t<T>::maybe_t(just_ctor_t c, T&& v)
    : value_(std::make_shared<T>(std::move(v))) { (void)c; }

    template < typename T >
    maybe_t<T>::maybe_t(just_ctor_t c, const T& v)
    : value_(std::make_shared<T>(v)) { (void)c; }

    template < typename T >
    template < typename... Us >
    maybe_t<T>::maybe_t(just_inplace_ctor_t c, Us&&... vs)
    : value_(std::make_shared<T>(std::forward<Us>(vs)...)) { (void)c; }

    template < typename T >
    bool maybe_t<T>::is_just() const noexcept {
        return !!value_;
    }

    template < typename T >
    bool maybe_t<T>::is_nothing() const noexcept {
        return !value_;
    }

    template < typename T >
    const T& maybe_t<T>::operator*() const {
        if ( is_nothing() ) {
            throw std::logic_error("nothing maybe_t");
        }
        return *value_.get();
    }

    template < typename T >
    const T* maybe_t<T>::operator->() const {
        if ( is_nothing() ) {
            throw std::logic_error("nothing maybe_t");
        }
        return value_.get();
    }
}
