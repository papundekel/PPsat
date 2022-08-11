#include <PPsat/antlrer.hpp>
#include <PPsat/builder.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener.hpp>
#include <PPsat/factory_lexer.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/literal_pair.hpp>
#include <PPsat/logger_subroutine.hpp>
#include <PPsat/renaming.hpp>
#include <PPsat/renaming_map.hpp>
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
                   const PPsat::formula& formula,
                   const PPsat::renaming& renaming,
                   const std::size_t count_variable)
{
    const auto count_clause = formula.clause_count();
    const auto count_variable_input = renaming.count();
    const auto count_variable_introduced =
        count_variable - count_variable_input;

    const auto new_native_begin = 1uz;
    const auto new_introduced_begin =
        greater_power_10(count_variable_input) + 1uz;

    output << "c The introduced variables begin at " << new_introduced_begin
           << ".\n";

    std::map<std::size_t, std::size_t> renaming_output;

    for (auto name_internal = 0uz,
              new_native = new_native_begin,
              new_introduced = new_introduced_begin;
         name_internal != count_variable;
         ++name_internal)
    {
        const auto rename = renaming.get_variable(name_internal);
        const auto is_native = rename.has_value();

        renaming_output.try_emplace(name_internal,
                                    is_native ? new_native : new_introduced++);

        if (is_native)
        {
            output << "c " << new_native << "->" << *rename << "\n";
            ++new_native;
        }
    }

    output << "p cnf " << count_variable << " " << count_clause << "\n";

    formula.write_DIMACS(
        output,
        [&renaming_output](std::ostream & output,
                           const PPsat::literal& l) -> auto& {
            output << PPsat::literal_pair{renaming_output.at(l.get_variable()),
                                          l.is_positive()};
            return output;
        });
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

    PPsat::formula_simple formula;
    const auto renaming = builder->create_ref();

    auto& input_formula = argument_file_in.parsed_stream();

    const auto result =
        builder->read(logger_inner, input_formula, formula, renaming);

    if (!result)
    {
        logger_inner << "Skipping outputting the formula.\n";

        return 2;
    }

    auto& output_formula = argument_file_out.parsed_stream();

    write_formula(output_formula,
                  formula,
                  renaming,
                  result.get_variable_count());

    return 0;
}
