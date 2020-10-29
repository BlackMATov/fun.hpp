/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "../basic/_basic.hpp"

namespace fun
{
    //
    // type_instance_t
    //

    template
    <
        template
        <
            typename,
            typename
        > class Instance
    >
    struct type_instance_t {
        template < typename A >
        using instance_type = Instance<A,A>;
        static constexpr bool instance = false;
    };

    //
    // template_type_instance_t
    //

    template
    <
        template
        <
            template<typename...> class,
            template<typename...> class
        > class Instance
    >
    struct template_type_instance_t {
        template < template <typename...> class A >
        using instance_type = Instance<A,A>;
        static constexpr bool instance = false;
    };

    //
    // type_class_t
    //

    template
    <
        template
        <
            typename,
            typename
        > class Instance
    >
    struct type_class_t {
        template < typename A >
        using instance_type = typename Instance<A,A>::template instance_type<A>;

        template < typename A >
        static constexpr bool instance = instance_type<A>::instance;

        template < typename A >
        static void check_instance() {
            static_assert(instance<A>,
                "A should be instance of this type class");
        }
    };

    //
    // template_type_class_t
    //

    template
    <
        template
        <
            template<typename...> class,
            template<typename...> class
        > class Instance
    >
    struct template_type_class_t {
        template < template <typename...> class A >
        using instance_type = typename Instance<A,A>::template instance_type<A>;

        template < template <typename...> class A >
        static constexpr bool instance = instance_type<A>::instance;

        template < template <typename...> class A >
        constexpr static void check_instance() {
            static_assert(instance<A>,
                "A should be instance of this type class");
        }
    };
}
