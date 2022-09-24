#pragma once
#include <PPsat/cli/argument/file.hpp>

#include <compare>
#include <iosfwd>
#include <optional>

namespace PPsat_base
{
class logger;
}

namespace PPsat::cli
{
class parameters_value;
}

namespace PPsat::subprogram
{
enum class selection
{
    convert,
    solve,
};

using type = int(const PPsat_base::logger&, const cli::parameters_value&);

type convert;
type solve;
type invalid;
}

namespace PPsat
{
constexpr std::strong_ordering operator<=>(const subprogram::selection& a,
                                           const subprogram::selection& b)
{
    return (int)a <=> (int)b;
}
}
