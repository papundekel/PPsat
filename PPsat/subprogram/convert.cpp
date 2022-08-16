#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause_simple.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/subprogram/convert.hpp>
#include <PPsat/variable_DIMACS.hpp>
#include <PPsat/variable_SMTLIB.hpp>
#include <PPsat/variable_simple.hpp>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/builder.hpp>
#include <PPsat-base/containers.hpp>
#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory_lexer.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/logger_subroutine.hpp>
#include <PPsat-base/variable.hpp>
#include <PPsat-base/variable_representation.hpp>
#include <PPsat-base/variable_representation_always.hpp>
#include <PPsat-base/variable_representation_maybe.hpp>

#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace
{
template <PPsat::formula_format format>
struct variable_ final
    : public PPsat::variable_simple
    , public std::conditional_t<format == PPsat::formula_format::DIMACS,
                                PPsat::variable_DIMACS,
                                PPsat::variable_SMTLIB>
{};

constexpr auto greater_power_10(const auto x) noexcept
{
    auto power = 10;

    while (x >= power)
    {
        power *= 10;
    }

    return power;
}

void write_formula(std::ostream& output, PPsat_base::formula& formula)
{
    const auto count_clause = formula.count_clause();
    const auto count_variable = formula.count_variable();

    std::size_t count_variable_native = 0;

    formula.for_each(
        [&count_variable_native](PPsat_base::variable& variable)
        {
            if (variable.representation_has())
            {
                ++count_variable_native;
            }
        });

    const auto count_variable_introduced =
        count_variable - count_variable_native;

    const auto new_native_begin = 1uz;
    const auto new_introduced_begin =
        greater_power_10(count_variable_native) + 1uz;

    output << "c The introduced variables begin at " << new_introduced_begin
           << ".\n";

    formula.for_each(
        [&output,
         new_native = new_native_begin,
         new_introduced =
             new_introduced_begin](PPsat_base::variable& variable) mutable
        {
            const auto is_native = variable.representation_has();

            if (is_native)
            {
                output << "c " << new_native << "->" << variable << "\n";
            }

            variable.representation_set(is_native ? new_native++
                                                  : new_introduced++);
        });

    output << "p cnf " << count_variable << " " << count_clause << "\n";

    formula.write_DIMACS(output);
}

std::unique_ptr<PPsat_base::formula::factory_variable> create_variables(
    PPsat::formula_format format)
{
    switch (format)
    {
        case PPsat::formula_format::DIMACS:
            return std::make_unique<PPsat_base::formula::factory_variable::impl<
                PPsat_base::list,
                variable_<PPsat::formula_format::DIMACS>>>();
        case PPsat::formula_format::SMTLIB:
            return std::make_unique<PPsat_base::formula::factory_variable::impl<
                PPsat_base::list,
                variable_<PPsat::formula_format::SMTLIB>>>();
        default:
            return nullptr;
    }
}
}

PPsat::subcommand_result PPsat::subprogram::convert_unparsed(
    const PPsat_base::logger& logger_outer,
    PPsat_base::cli::arguments& arguments,
    cli::options& options)
{
    if (!options.convert)
    {
        return {};
    }

    const auto logger_inner =
        PPsat_base::logger_subroutine(logger_outer, "convert");

    PPsat::cli::argument::file_in argument_file_in;
    PPsat::cli::argument::file_out argument_file_out;

    const auto success = arguments.parse(
        logger_inner,
        std::array<std::reference_wrapper<PPsat_base::cli::argument_>, 2>{
            argument_file_in,
            argument_file_out});

    if (!success)
    {
        logger_inner << "Skipping the subprogram.\n";

        return 1;
    }

    const auto format =
        pick_format(options.format, argument_file_in, formula_format::SMTLIB);

    const auto builder = create_builder(format, options.nnf);

    PPsat_base::formula::factory_clause::impl<PPsat_base::list, clause_simple>
        clauses;
    auto variables = create_variables(format);
    PPsat_base::formula formula(clauses, *variables);

    auto& input_formula = argument_file_in.parsed_stream();

    const auto result = builder->read(logger_inner, input_formula, formula);

    if (!result)
    {
        logger_inner << "Skipping outputting the formula.\n";

        return 2;
    }

    auto& output_formula = argument_file_out.parsed_stream();

    write_formula(output_formula, formula);

    return 0;
}
