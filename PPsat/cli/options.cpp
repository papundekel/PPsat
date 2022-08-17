#include "PPsat/subprogram.hpp"
#include <PPsat/cli/options.hpp>

#include <PPsat/clause_type.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/view_any.hpp>

using namespace std::literals;

PPsat::cli::options::options()
    : help("help")
    , subprogram(
          "subprogram",
          std::array{std::make_pair("convert", subprogram::selection::convert),
                     std::make_pair("dpll", subprogram::selection::dpll),
                     std::make_pair("cdcl", subprogram::selection::cdcl)})
    , nnf("nnf")
    , watched_literals("wl")
    , format("format",
             std::array{std::make_pair("dimacs", formula_format::DIMACS),
                        std::make_pair("smtlib", formula_format::SMTLIB)})
    , clause("clause",
             std::array{std::make_pair("sets", clause_type::sets),
                        std::make_pair("counting", clause_type::counting)})
{}

std::array<std::reference_wrapper<PPsat_base::cli::option_>, 6>
PPsat::cli::options::as_range() noexcept
{
    return {help, subprogram, nnf, watched_literals, format, clause};
}
