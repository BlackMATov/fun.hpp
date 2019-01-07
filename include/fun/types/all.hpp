/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.hpp"

namespace fun
{
    class all_t {
    public:
        all_t(bool value) noexcept
        : value_(value) {}

        bool get_all() const noexcept {
            return value_;
        }
    private:
        bool value_;
    };
}

namespace fun
{
    inline auto make_all() noexcept {
        return all_t(true);
    }

    inline auto make_all(bool value) noexcept {
        return all_t(value);
    }
}
