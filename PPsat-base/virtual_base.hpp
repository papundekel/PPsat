#pragma once
#include <PPsat-base/value_t.hpp>

namespace PPsat_base
{
template <bool virtual_, auto... Base>
class virtual_base;

template <auto... Base>
class virtual_base<false, Base...> : public decltype(Base)::type...
{};

template <auto... Base>
class virtual_base<true, Base...> : public virtual decltype(Base)::type...
{};

template <bool virtual_, auto B, auto... Bases>
class virtual_bases;

template <auto B, auto... Bases>
class virtual_bases<true, B, Bases...>
    : public decltype(Bases(value_v<true>, B))::type...
{};

template <auto B>
class virtual_bases<false, B> : public decltype(B)::type
{};

template <auto B, auto B0, auto... BTail>
class virtual_bases<false, B, B0, BTail...>
    : public virtual_bases<false, B0(value_v<false>, B), BTail...>
{};
}
