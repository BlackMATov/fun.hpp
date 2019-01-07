/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>

namespace fun
{
    struct box_ctor_t {
        constexpr explicit box_ctor_t() = default;
    };

    struct box_inplace_ctor_t {
        constexpr explicit box_inplace_ctor_t() = default;
    };

    template < typename T >
    class box_t final {
    public:
        box_t();
        ~box_t() noexcept = default;

        box_t(const box_t&) = default;
        box_t& operator=(const box_t&) = default;

        explicit box_t(box_ctor_t c, T&& v);
        explicit box_t(box_ctor_t c, const T& v);

        template < typename... Us >
        explicit box_t(box_inplace_ctor_t c, Us&&... vs);

        const T& operator*() const;
        const T* operator->() const;
    private:
        std::shared_ptr<T> value_;
    };
}

namespace fun
{
    template < typename T >
    auto make_box() {
        return box_t<T>(box_ctor_t{}, T{});
    }

    template < typename T >
    auto make_box(T&& v) {
        using NT = std::decay_t<T>;
        return box_t<NT>(box_ctor_t{}, std::forward<T>(v));
    }

    template < typename T, typename... Us >
    auto make_box_inplace(Us&&... vs) {
        return box_t<T>(box_inplace_ctor_t{}, std::forward<Us>(vs)...);
    }
}

namespace fun
{
    template < typename T >
    box_t<T>::box_t()
    : value_(std::make_shared<T>()) {}

    template < typename T >
    box_t<T>::box_t(box_ctor_t c, T&& v)
    : value_(std::make_shared<T>(std::move(v))) { (void)c; }

    template < typename T >
    box_t<T>::box_t(box_ctor_t c, const T& v)
    : value_(std::make_shared<T>(v)) { (void)c; }

    template < typename T >
    template < typename... Us >
    box_t<T>::box_t(box_inplace_ctor_t c, Us&&... vs)
    : value_(std::make_shared<T>(std::forward<Us>(vs)...)) { (void)c; }

    template < typename T >
    const T& box_t<T>::operator*() const {
        return *value_.get();
    }

    template < typename T >
    const T* box_t<T>::operator->() const {
        return value_.get();
    }
}
