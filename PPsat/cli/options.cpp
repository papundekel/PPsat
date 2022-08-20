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
          subprogram::selection::dpll,
          std::array{std::make_pair("convert", subprogram::selection::convert),
                     std::make_pair("dpll", subprogram::selection::dpll),
                     std::make_pair("cdcl", subprogram::selection::cdcl)})
    , nnf("nnf")
    , format("format",
             formula_format::DIMACS,
             std::array{std::make_pair("dimacs", formula_format::DIMACS),
                        std::make_pair("smtlib", formula_format::SMTLIB)})
    , clause("clause",
             clause_type::sets,
             std::array{std::make_pair("sets", clause_type::sets),
                        std::make_pair("watched_literals",
                                       clause_type::watched_literals),
                        std::make_pair("counting", clause_type::counting)})
    , adjacency("adjacency",
                adjacency_type::vector,
                std::array{std::make_pair("vector", adjacency_type::vector),
                           std::make_pair("list", adjacency_type::list),
                           std::make_pair("set", adjacency_type::set),
                           std::make_pair("set_unordered",
                                          adjacency_type::set_unordered)})
{}

std::array<std::reference_wrapper<PPsat_base::cli::option_>, 6>
PPsat::cli::options::as_range() noexcept
{
    return {help, subprogram, nnf, format, clause, adjacency};
}
