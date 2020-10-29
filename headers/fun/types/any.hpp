/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.hpp"

namespace fun
{
    class any_t {
    public:
        any_t(bool value) noexcept
        : value_(value) {}

        bool get_any() const noexcept {
            return value_;
        }
    private:
        bool value_;
    };
}

namespace fun
{
    inline auto make_any() noexcept {
        return any_t(false);
    }

    inline auto make_any(bool value) noexcept {
        return any_t(value);
    }
}
