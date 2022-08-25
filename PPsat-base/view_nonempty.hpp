#pragma once
#include <utility>

namespace PPsat_base
{
constexpr inline auto view_nonempty = [](auto&& view)
{
    for (auto&& _ : std::forward<decltype(view)>(view))
    {
        return true;
    }
    return false;
};
}
