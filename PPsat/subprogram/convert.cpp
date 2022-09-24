#include <PPsat/adjacency_none.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause_simple.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/creator.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/formula_impl.hpp>
#include <PPsat/preprocessor.hpp>
#include <PPsat/preprocessor_basic.hpp>
#include <PPsat/preprocessor_id.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_adjacency.hpp>
#include <PPsat/variable_antecedent_none.hpp>
#include <PPsat/variable_assignable_not.hpp>
#include <PPsat/variable_level_none.hpp>
#include <PPsat/variable_picker.hpp>
#include <PPsat/variable_recency_none.hpp>
#include <PPsat/variable_representation.hpp>
#include <PPsat/variable_representation_always.hpp>
#include <PPsat/variable_representation_maybe.hpp>
#include <PPsat/variable_score_none.hpp>
#include <PPsat/variable_score_with.hpp>
#include <PPsat/variable_unassigning.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory_lexer.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
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

void write_formula(std::ostream& output, PPsat::formula& formula)
{
    const auto count_clause = formula.count_clause();
    const auto count_variable = formula.count_variable();

    std::size_t count_variable_native = 0;

    formula.for_each_variable(
        [&count_variable_native](const PPsat::variable& variable)
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

    std::unordered_map<PPsat::variable*, std::size_t> renaming;

    formula.for_each_variable(
        [&output,
         new_native = new_native_begin,
         new_introduced = new_introduced_begin,
         &renaming](PPsat::variable& variable) mutable
        {
            const auto is_native = variable.representation_has();

            if (is_native)
            {
                output << "c " << new_native << "->" << variable << "\n";
            }

            renaming.try_emplace(&variable,
                                 is_native ? new_native++ : new_introduced++);
        });

    output << "p cnf " << count_variable << " " << count_clause << "\n";

    formula.write_DIMACS(
        output,
        [&renaming](std::ostream & output,
                    const PPsat::literal literal) -> auto& {
            if (!literal.is_positive())
            {
                output << "-";
            }
            output << renaming.at(&literal.get_variable());
            return output;
        });
}
}

int PPsat::subprogram::convert(const PPsat_base::logger& logger_outer,
                               const cli::parameters_value& parameters)
{
    const auto logger_inner =
        PPsat_base::logger_subroutine(logger_outer, "convert");

    const auto& creator = creator::pick(parameters);

    const preprocessor_basic preprocessor;
    const auto clauses_factory = creator.clauses_factory();
    const auto clauses = clauses_factory->create();
    const auto variables = creator.variables();
    const auto formula = creator.formula(preprocessor, clauses, variables);

    antlr4::ANTLRInputStream input_formula(parameters.input.second);

    const auto [builder, renaming] =
        create_builder(formula, parameters.format_input, parameters.nnf);
    const auto result = builder->read(logger_inner, input_formula, true);

    if (!result)
    {
        logger_inner << "Skipping outputting the formula.\n";

        return 1;
    }

    write_formula(parameters.output.second, formula);

    return 0;
}
