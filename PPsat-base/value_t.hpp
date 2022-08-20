#pragma once

namespace PPsat_base
{
template <auto Value>
class value_t
{
public:
    static constexpr auto value = Value;
};
template <auto Value>
constexpr inline value_t<Value> value_v;
}
