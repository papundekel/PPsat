#pragma once
#include <ranges>

namespace PPsat_base
{
template <template <typename> typename C>
constexpr inline auto ranges_to = [](auto&& range)
{
    auto begin = [&range]() -> decltype(auto)
    {
        return std::begin(std::forward<decltype(range)>(range));
    };

    using E = decltype(*begin());
    return C<E>(begin(), std::end(std::forward<decltype(range)>(range)));
};
}
