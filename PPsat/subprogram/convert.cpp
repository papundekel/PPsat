#include <PPsat/antlrer.hpp>
#include <PPsat/builder_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/literal_pair.hpp>
#include <PPsat/renaming_map.hpp>
#include <PPsat/reader_SMTLIB_tseitin.hpp>
#include <PPsat/subprogram/convert.hpp>
#include <PPsat/tseitin_builder_nnf.hpp>

#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace
{
constexpr auto greater_power_10(auto x) noexcept
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
                   std::size_t count_variable)
{
    const auto count_clause = formula.clause_count();
    const auto count_variable_input = renaming.count();
    const auto count_variable_introduced =
        count_variable - count_variable_input;

    const auto new_native_begin = 1uz;
    const auto new_introduced_begin =
        greater_power_10(count_variable_input) + 1;

    output << "c The introduced variables begin at " << new_introduced_begin
           << ".\n";

    for (auto name_internal = 0uz, new_native = new_native_begin;
         name_internal != count_variable;
         ++name_internal)
    {
        auto name_input_opt = renaming.get(name_internal);

        if (name_input_opt)
        {
            output << "c " << new_native << "->" << *name_input_opt << "\n";
            ++new_native;
        }
    }

    output << "p cnf " << count_clause << " " << count_variable << "\n";

    formula.write_DIMACS(
        output,
        [
            &renaming,
            new_native = new_native_begin,
            new_introduced = new_introduced_begin
        ](std::ostream & output, const PPsat::literal& l) mutable -> auto& {
            const auto name_internal = l.get_variable();

            const auto is_native = renaming.get(name_internal).has_value();

            output << PPsat::literal_pair{is_native ? new_native++
                                                    : new_introduced++,
                                          l.is_positive()};

            return output;
        });
}
}

PPsat::subcommand_result PPsat::subprogram::convert_unparsed(
    cli::arguments& arguments,
    options& options)
{
    if (!options.convert)
    {
        return {};
    }

    PPsat::cli::argument::file_in argument_in;
    PPsat::cli::argument::file_out argument_out;

    bool success = arguments.parse(
        std::array<std::reference_wrapper<PPsat::cli::argument_>, 2>{
            argument_in,
            argument_out});

    return convert(argument_in.parsed_stream(),
                   argument_out.parsed_stream(),
                   std::cerr,
                   options.nnf);
}

int PPsat::subprogram::convert(std::istream& input,
                               std::ostream& output,
                               std::ostream& err,
                               bool nnf)
{
    renaming_map renaming;
    formula_simple formula;

    write_formula(output, formula, renaming, count_variable);

    return 0;
}
