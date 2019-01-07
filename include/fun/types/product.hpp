/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_types.hpp"

#include <utility>

namespace fun
{
    template < typename A >
    class product_t {
    public:
        product_t(A&& value)
        : value_(std::move(value)) {}

        product_t(const A& value)
        : value_(value) {}

        const A& get_product() const noexcept {
            return value_;
        }
    private:
        A value_;
    };
}

namespace fun
{
    template < typename A >
    auto make_product() {
        return product_t<A>(A());
    }

    template < typename A >
    auto make_product(A&& value) {
        using NA = std::decay_t<A>;
        return product_t<NA>(std::forward<A>(value));
    }

    template < typename A >
    auto make_product(const A& value) {
        return product_t<A>(value);
    }

    struct make_product_f {
        template < typename A >
        auto operator()(A&& value) const {
            return make_product(std::forward<A>(value));
        }
    };
    inline const auto fproduct = curry(make_product_f());
}
