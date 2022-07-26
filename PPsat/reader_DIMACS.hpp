#include <PPsat/CNF_simple.hpp>
#include <PPsat/literal_pair.hpp>

#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>

#include <charconv>
#include <list>
#include <map>
#include <optional>

namespace
{
auto greater_power10(auto x)
{
    decltype(x) power = 1;

    while (power <= x)
    {
        power *= 10;
    }

    return power;
}
}

namespace PPsat
{
template <typename FormulaBuilder>
class reader_DIMACS final : public parser_DIMACSVisitor
{
    std::size_t variable_count;
    FormulaBuilder& formula_builder;
    std::vector<literal_pair<std::size_t>> clause;
    bool parse_error;

public:
    reader_DIMACS(FormulaBuilder& formula_builder) noexcept
        : variable_count(0)
        , formula_builder(formula_builder)
        , clause()
        , parse_error(false)
    {}

private:
    std::optional<std::size_t> parse_number(
        antlr4::tree::TerminalNode* number_node)
    {
        std::size_t number;

        const auto number_string = context->NUMBER(0)->getSymbol()->getText();

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

    std::any visitInput(parser_DIMACS::InputContext* context)
    {
        const auto variable_count_opt = parse_number(context->NUMBER(0));

        if (!variable_count_opt)
        {
            return {};
        }

        for (auto* const clause_context : context->clause())
        {
            visit(clause_context);
        }

        return {};
    }

    std::any visitClause(parser_DIMACS::ClauseContext* context)
    {
        for (auto* const literal_context : context->literal())
        {
            visit(literal_context);
        }

        formula_builder.push_clause(std::move(clause));
        clause.clear();

        return {};
    }

    std::any visitLiteral(parser_DIMACS::Signed_numberContext* context)
    {
        const auto variable_name_opt = parse_number(context->NUMBER());

        if (!variable_name_opt)
        {
            return {};
        }

        clause.push_back({*variable_name_opt, context->MINUS() == nullptr});

        return {};
    }

public:
    std::optional<std::size_t> result(parser_DIMACS::InputContext* context) &&
    {
        visit(context);

        if (parse_error)
            return std::nullopt;

        return variable_count;
    }
};
}
