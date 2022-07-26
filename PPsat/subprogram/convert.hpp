#pragma once
#include <PPsat/cli/arguments.hpp>
#include <PPsat/options.hpp>
#include <PPsat/subcommand_result.hpp>

#include <iosfwd>
#include <string_view>
#include <vector>

namespace PPsat::subprogram
{
subcommand_result convert_unparsed(cli::arguments& arguments, options& options);

int convert(std::istream& input,
            std::ostream& output,
            std::ostream& err,
            bool nnf);
}
