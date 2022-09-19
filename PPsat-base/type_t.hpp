#pragma once

namespace PPsat_base
{
template <typename T>
class type_t
{
public:
    using type = T;
};

template <typename T>
constexpr inline type_t<T> type;
}
