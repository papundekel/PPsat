#include "PPsat/clause_simple.hpp"
#include "PPsat/containers.hpp"
#include "PPsat/formula.hpp"
#include "PPsat/variable.hpp"
#include "PPsat/variable_simple.hpp"
#include <PPsat/antlrer.hpp>
#include <PPsat/builder.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener.hpp>
#include <PPsat/factory_lexer.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/logger_subroutine.hpp>
#include <PPsat/subprogram/convert.hpp>

#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace
{
constexpr auto greater_power_10(const auto x) noexcept
{
    auto power = 10;

    while (x >= power)
    {
        power *= 10;
    }

    return power;
}

void write_formula(std::ostream& output,
                   PPsat::formula& formula,
                   const std::size_t count_variable)
{
    const auto count_clause = formula.clause_count();

    std::size_t count_variable_native = 0;

    formula.for_each(
        [&count_variable_native](PPsat::variable& variable)
        {
            if (variable.has_input_name())
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
             new_introduced_begin](PPsat::variable& variable) mutable
        {
            const auto is_native = variable.has_input_name();

            variable.set_input_name(is_native ? new_native : new_introduced++);

            if (is_native)
            {
                output << "c " << new_native << "->" << variable << "\n";
                ++new_native;
            }
        });

    output << "p cnf " << count_variable << " " << count_clause << "\n";

    formula.write_DIMACS(output);
}
}

PPsat::subcommand_result PPsat::subprogram::convert_unparsed(
    const logger& logger_outer,
    cli::arguments& arguments,
    options& options)
{
    if (!options.convert)
    {
        return {};
    }

    const auto logger_inner = logger_subroutine(logger_outer, "convert");

    PPsat::cli::argument::file_in argument_file_in;
    PPsat::cli::argument::file_out argument_file_out;

    const auto success = arguments.parse(
        logger_inner,
        std::array<std::reference_wrapper<PPsat::cli::argument_>, 2>{
            argument_file_in,
            argument_file_out});

    if (!success)
    {
        logger_inner << "Skipping the subprogram.\n";

        return 1;
    }

    const auto builder =
        create_builder(options.format, argument_file_in, options.nnf);

    PPsat::formula::factory_clause::impl<PPsat::list, clause_simple> clauses;
    PPsat::formula::factory_variable::impl<PPsat::list, variable_simple>
        variables;
    PPsat::formula formula(clauses, variables);

    auto& input_formula = argument_file_in.parsed_stream();

    const auto result = builder->read(logger_inner, input_formula, formula);

    if (!result)
    {
        logger_inner << "Skipping outputting the formula.\n";

        return 2;
    }

    auto& output_formula = argument_file_out.parsed_stream();

    write_formula(output_formula, formula, result.get_variable_count());

    return 0;
}
