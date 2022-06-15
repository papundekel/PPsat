#pragma once
#include <concepts>

namespace PPsat
{
    template <typename L>
    concept literal = requires (L l)
    {
        l.variable;
        { l.is_positive() } -> std::convertible_to<bool>;
        typename L::name_t;
    };
}
