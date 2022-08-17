#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/options.hpp>
#include <PPsat/subprogram.hpp>

#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <array>
#include <iostream>
#include <istream>
#include <ostream>

namespace
{
int cdcl_impl(std::istream& input,
              std::ostream& output,
              const PPsat_base::logger& logger_outer,
              const PPsat::formula_format format,
              const bool watched_literals,
              const bool nnf)
{
    return 0;
}
}

int PPsat::subprogram::cdcl(const PPsat_base::logger& logger_outer,
                            cli::options& options,
                            cli::argument::file_in& argument_file_in,
                            cli::argument::file_out&)
{
    const auto& logger_inner =
        PPsat_base::logger_subroutine(logger_outer, "cdcl");

    return cdcl_impl(argument_file_in.parsed_stream(),
                     std::cout,
                     logger_inner,
                     options.format     ? options.format.parsed()
                     : argument_file_in ? argument_file_in.parsed_format()
                                        : formula_format::DIMACS,
                     options.watched_literals,
                     options.nnf);
}
