#pragma once
#include <ranges>
#include <type_traits>
#include <utility>

namespace PPsat_base
{
template <template <typename...> typename C, typename... P>
constexpr inline auto ranges_to = [](auto&& range)
{
    using E = std::remove_cvref_t<decltype(*std::ranges::begin(range))>;

    C<E, P...> container;

    for (auto&& e : std::forward<decltype(range)>(range))
    {
        container.emplace_back(std::forward<decltype(e)>(e));
    }

    return container;
};
}
