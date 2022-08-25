#include "PPsat-base/logger_subroutine.hpp"
#include <PPsat/visitor_DIMACS.hpp>

#include <PPsat-base/formula.hpp>

#include <charconv>

PPsat::visitor_DIMACS::visitor_DIMACS(
    const PPsat_base::logger& logger_outer,
    PPsat_base::formula& formula,
    renaming_int& renaming_from_input) noexcept
    : logger(logger_outer, "visitor-DIMACS")
    , formula(formula)
    , renaming_from_input(renaming_from_input)
    , name_next(0)
{}

std::any PPsat::visitor_DIMACS::visitInput(parser_DIMACS::InputContext* context)
{
    for (auto* const clause_context : context->clause())
    {
        const auto result = visit(clause_context);
        if (!result.has_value())
        {
            return {};
        }
    }

    return nullptr;
}

std::any PPsat::visitor_DIMACS::visitClause(
    parser_DIMACS::ClauseContext* context)
{
    std::vector<PPsat_base::literal> clause;

    for (auto* const literal_context : context->literal())
    {
        const auto literal_any = visit(literal_context);

        if (!literal_any.has_value())
        {
            return {};
        }

        clause.push_back(std::any_cast<PPsat_base::literal>(literal_any));
    }

    formula.add_clause(PPsat_base::view_any<PPsat_base::literal>(clause));

    return nullptr;
}

std::any PPsat::visitor_DIMACS::visitLiteral(
    parser_DIMACS::LiteralContext* context)
{
    const auto name_input_str = context->NUMBER()->getText();
    const auto name_input_parsed_opt =
        renaming_int::parse_number(name_input_str);

    if (!name_input_parsed_opt)
    {
        logger << "Couldn't parse \"" << name_input_str << "\" as a number\n";
        return {};
    }

    auto& variable = [this](std::size_t name_input) -> auto&
    {
        const auto variable_opt = renaming_from_input.contains(name_input);

        if (variable_opt)
        {
            return *variable_opt;
        }

        auto& variable_new = formula.create_new_variable();

        variable_new.representation_set(name_input);
        renaming_from_input.emplace(name_input, variable_new);

        return variable_new;
    }
    (*name_input_parsed_opt);

    return PPsat_base::literal{variable, context->NEGATED() == nullptr};
}
