#include <PPsat/literal_pair.hpp>
#include <PPsat/visitor_DIMACS.hpp>

#include <charconv>

PPsat::visitor_DIMACS::visitor_DIMACS(formula& formula,
                                      renaming& renaming_from_native) noexcept
    : f(formula)
    , renaming_from_input()
    , renaming_from_native(renaming_from_native)
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
    std::vector<literal_pair> clause;

    for (auto* const literal_context : context->literal())
    {
        const auto literal_any = visit(literal_context);

        if (!literal_any.has_value())
        {
            return {};
        }

        clause.push_back(std::any_cast<literal_pair>(literal_any));
    }

    f.add_clause(view_literal(clause));

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

    const auto name_native = [this](std::size_t name_input)
    {
        const auto name_input_renaming_i = renaming_from_input.find(name_input);

        if (name_input_renaming_i != renaming_from_input.end())
        {
            return name_input_renaming_i->second;
        }
        else
        {
            const auto name_native = name_next++;

            renaming_from_native.rename(name_native, name_input);
            renaming_from_input.try_emplace(name_input, name_native);

            return name_native;
        }
    }(*name_input_opt);

    return literal_pair{name_native, context->NEGATED() == nullptr};
}
