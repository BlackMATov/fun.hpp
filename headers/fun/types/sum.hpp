/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.hpp"

namespace fun
{
    template < typename A >
    class sum_t {
    public:
        sum_t(A&& value)
        : value_(std::move(value)) {}

        sum_t(const A& value)
        : value_(value) {}

        const A& get_sum() const noexcept {
            return value_;
        }
    private:
        A value_;
    };
}

namespace fun
{
    template < typename A >
    auto make_sum() {
        return sum_t<A>(A());
    }

    template < typename A >
    auto make_sum(A&& value) {
        using NA = std::decay_t<A>;
        return sum_t<NA>(std::forward<A>(value));
    }

    template < typename A >
    auto make_sum(const A& value) {
        return sum_t<A>(value);
    }

    struct make_sum_f {
        template < typename A >
        auto operator()(A&& value) const {
            return make_sum(std::forward<A>(value));
        }
    };
    inline const auto fsum = curry(make_sum_f());
}
