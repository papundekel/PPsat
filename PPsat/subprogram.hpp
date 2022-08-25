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
class options;
}

namespace PPsat::subprogram
{
enum class selection
{
    convert,
    solve,
};

using type = int(const PPsat_base::logger&,
                 cli::options&,
                 cli::argument::file_in&,
                 cli::argument::file_out&);

type convert;
type solve;
}
