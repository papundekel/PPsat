#include <PPsat/cli/options.hpp>

#include <PPsat/clause_type.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/view_any.hpp>

using namespace std::literals;

PPsat::cli::options::options()
    : help("help")
    , convert("convert")
    , dpll("dpll")
    , cdcl("cdcl")
    , nnf("nnf")
    , watched_literals("wl")
    , format("format",
             std::array{std::make_pair("dimacs", formula_format::DIMACS),
                        std::make_pair("smtlib", formula_format::SMTLIB)})
    , clause("clause",
             std::array{std::make_pair("sets", clause_type::sets),
                        std::make_pair("counting", clause_type::counting)})
{}

std::array<std::reference_wrapper<PPsat_base::cli::option_>, 8>
PPsat::cli::options::as_range() noexcept
{
    return {help, convert, dpll, cdcl, nnf, watched_literals, format, clause};
}
