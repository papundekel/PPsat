#pragma once
#include <PPsat/cli/options.hpp>
#include <PPsat/subcommand_result.hpp>

#include <PPsat-base/cli/arguments.hpp>
#include <PPsat-base/logger.hpp>

namespace PPsat::subprogram
{
void help_print(std::ostream& out);

subcommand_result help_unparsed(const PPsat_base::logger& logger_outer,
                                PPsat_base::cli::arguments& arguments,
                                cli::options& options);
}
