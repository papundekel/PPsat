#include <PPsat/formula.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/literal_pair.hpp>
#include <PPsat/renaming.hpp>

#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>

#include <charconv>
#include <list>
#include <map>
#include <optional>

namespace PPsat
{
class visitor_DIMACS final : public parser_DIMACSVisitor
{
    formula& f;
    std::map<std::string, std::size_t> renaming_internal;
    renaming& renaming_external;
    bool parse_error;

public:
    visitor_DIMACS(formula& formula, renaming& renaming_external) noexcept
        : f(formula)
        , renaming_internal()
        , renaming_external(renaming_external)
        , parse_error(false)
    {}

private:
    std::optional<std::size_t> parse_number(
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

    std::any visitInput(parser_DIMACS::InputContext* context)
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
        }

        return *variable_count_opt;
    }

    std::any visitClause(parser_DIMACS::ClauseContext* context)
    {
        std::vector<literal_pair> clause;

        for (auto* const literal_context : context->literal())
        {
            clause.push_back(
                std::any_cast<literal_pair>(visit(literal_context)));
        }

        f.add_clause(view_literal(clause));

        return {};
    }

    std::any visitLiteral(parser_DIMACS::LiteralContext* context)
    {
        const auto variable_name_opt = parse_number(context->NUMBER());

        // TODO
        if (!variable_name_opt)
        {
            return {};
        }

        return literal_pair{*variable_name_opt, context->MINUS() == nullptr};
    }
};
}
