/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "fun/types/box.hpp"
using namespace fun;

namespace
{
    struct obj_t {
        int v;
        obj_t() : v(42) {}
        obj_t(int nv) : v(nv) {}
        obj_t(int a, int b) : v(a + b) {}
    };
}

TEST_CASE("box") {
    {
        auto m = make_box<obj_t>();
        REQUIRE((*m).v == 42);
        REQUIRE(m->v == 42);
    }
    {
        auto m = make_box<obj_t>(84);
        REQUIRE((*m).v == 84);
        REQUIRE(m->v == 84);
    }
    {
        auto m = make_box_inplace<obj_t>(100, 42);
        REQUIRE((*m).v == 142);
        REQUIRE(m->v == 142);
    }
    {
        auto m = box_t<obj_t>();
        REQUIRE((*m).v == 42);
        REQUIRE(m->v == 42);
    }
}
