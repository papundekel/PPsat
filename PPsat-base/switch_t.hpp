#pragma once
#include <PPsat-base/type_t.hpp>

#include <concepts>
#include <utility>

namespace PPsat_base
{
namespace detail
{
template <auto key, std::pair... Mapping>
class switch_impl;

template <auto key, std::pair MappingHead, std::pair... Mapping>
    requires(MappingHead.first == key)
class switch_impl<key, MappingHead, Mapping...>
{
public:
    static constexpr auto value = MappingHead.second;
};

template <auto key, std::pair MappingHead, std::pair... Mapping>
class switch_impl<key, MappingHead, Mapping...>
    : public switch_impl<key, Mapping...>
{};
}

template <auto key, std::pair... Mapping>
constexpr inline auto switch_ = detail::switch_impl<key, Mapping...>::value;
}
