#pragma once
#include <utility>

namespace PPsat
{
constexpr auto partial_apply(auto&& f, auto&&... args1)
{
    return [f = std::forward<decltype(f)>(f),
            ... args1 = std::forward<decltype(args1)>(args1)](
               auto&&... args2) -> decltype(auto)
    {
        return f(args1..., std::forward<decltype(args2)>(args2)...);
    };
}
}
