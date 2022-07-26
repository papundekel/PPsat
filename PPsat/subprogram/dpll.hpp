#pragma once
#include <PPsat/cli/arguments.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/options.hpp>
#include <PPsat/subcommand_result.hpp>

#include <iosfwd>
#include <string_view>
#include <vector>

namespace PPsat::subprogram
{
subcommand_result dpll_unparsed(cli::arguments& arguments, options& options);

int dpll(std::istream& input,
         std::ostream& output,
         std::ostream& err,
         formula_format format,
         bool watched_literals,
         bool nnf);
}
