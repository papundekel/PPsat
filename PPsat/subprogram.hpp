#pragma once
#include <PPsat/cli/argument/file.hpp>

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
