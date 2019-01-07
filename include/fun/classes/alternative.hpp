/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/fun.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once
#include "_classes.hpp"

#include <type_traits>

#include "../classes/applicative.hpp"

namespace fun
{
    //
    // alternative_inst_t
    //

    template
    <
        template <typename...> class T,
        template <typename...> class = std::void_t
    >
    struct alternative_inst_t : template_type_instance_t<alternative_inst_t> {
        // ---------------------------------------------------------------------
        // -= Minimal for T<A> =-
        //
        // template < typename A >
        // static T<A> empty();
        //
        // template < typename A >
        // static T<A> alter(const T<A>& lhs, const T<A>& rhs);
        // ---------------------------------------------------------------------
    };

    //
    // alternative_t
    //

    using alternative_t = template_type_class_t<alternative_inst_t>;

    //
    // alternative_f
    //

    namespace alternative_f
    {
        //
        // empty
        //

        template
        <
            template <typename...> class T,
            typename... As
        >
        auto empty() {
            alternative_t::check_instance<T>();
            return alternative_t::instance_type<T>::template empty<As...>();
        }

        template < template <typename...> class T, typename... As >
        struct empty_f {
            auto operator()() const {
                return empty<T,As...>();
            }
        };

        template < template <typename...> class T, typename... As >
        inline const auto fempty = curry(empty_f<T,As...>());

        //
        // alter
        //

        template
        <
            template <typename...> class T,
            typename... As
        >
        auto alter(const T<As...>& lhs, const T<As...>& rhs) {
            alternative_t::check_instance<T>();
            return alternative_t::instance_type<T>::alter(lhs, rhs);
        }

        struct alter_f {
            template < template <typename...> class T, typename... As >
            auto operator()(const T<As...>& lhs, const T<As...>& rhs) const {
                return alter(lhs, rhs);
            }
        };
        inline const auto falter = curry(alter_f());
    }

    //
    // alternative operators
    //

    namespace alternative_ops
    {
        template
        <
            template <typename...> class T,
            typename... As,
            typename = std::enable_if<alternative_t::instance<T>>
        >
        auto operator||(const T<As...>& lhs, const T<As...>& rhs) {
            return alternative_f::alter(lhs, rhs);
        }
    }
}
