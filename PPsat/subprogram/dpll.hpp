#pragma once
#include <PPsat/cli/arguments.hpp>
#include <PPsat/logger.hpp>
#include <PPsat/options.hpp>
#include <PPsat/subcommand_result.hpp>

namespace PPsat::subprogram
{
subcommand_result dpll_unparsed(const logger& logger_outer,
                                cli::arguments& arguments,
                                options& options);
}
