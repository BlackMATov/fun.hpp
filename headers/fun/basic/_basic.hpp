/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <tuple>
#include <memory>
#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "../3rdparty/kari.hpp"

namespace fun
{
    using namespace kari;

    enum class ordering {
        lt,
        eq,
        gt
    };
}
