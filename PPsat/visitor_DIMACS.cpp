#include "PPsat-base/formula.hpp"
#include <PPsat/visitor_DIMACS.hpp>

#include <charconv>

PPsat::visitor_DIMACS::visitor_DIMACS(PPsat_base::formula& formula) noexcept
    : f(formula)
    , renaming_from_input()
    , name_next(0)
    , parse_error(false)
{}

std::optional<std::size_t> PPsat::visitor_DIMACS::parse_number(
    antlr4::tree::TerminalNode* number_node)
{
    std::size_t number;

    const auto number_string = number_node->getSymbol()->getText();

    const auto result =
        std::from_chars(number_string.data(),
                        number_string.data() + number_string.size(),
                        number);

    if (result.ec != std::errc())
    {
        parse_error = true;
        return std::nullopt;
    }

    return number;
}

std::any PPsat::visitor_DIMACS::visitInput(parser_DIMACS::InputContext* context)
{
    const auto variable_count_opt = parse_number(context->NUMBER(0));

    // TODO
    if (!variable_count_opt)
    {
        return {};
    }

    for (auto* const clause_context : context->clause())
    {
        visit(clause_context);

        if (parse_error)
        {
            return {};
        }
    }

    return *variable_count_opt;
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

    f.add_clause(PPsat_base::view_literal(clause));

    return {};
}

std::any PPsat::visitor_DIMACS::visitLiteral(
    parser_DIMACS::LiteralContext* context)
{
    const auto name_input_opt = parse_number(context->NUMBER());

    // TODO
    if (!name_input_opt)
    {
        return {};
    }

    auto& variable = [this](std::size_t name_input) -> auto&
    {
        const auto name_input_renaming_i = renaming_from_input.find(name_input);

        if (name_input_renaming_i != renaming_from_input.end())
        {
            return name_input_renaming_i->second;
        }
        else
        {
            auto& variable_new = f.create_new_variable();

            variable_new.representation_set(name_input);
            renaming_from_input.try_emplace(name_input, variable_new);

            return variable_new;
        }
    }
    (*name_input_opt);

    return PPsat_base::literal{variable, context->NEGATED() == nullptr};
}
