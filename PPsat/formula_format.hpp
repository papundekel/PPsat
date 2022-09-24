#pragma once

#include <compare>
namespace PPsat
{
enum class formula_format
{
    DIMACS,
    SMTLIB,
    unspecified,
};

constexpr std::strong_ordering operator<=>(const formula_format& a,
                                           const formula_format& b)
{
    return (int)a <=> (int)b;
}
}
