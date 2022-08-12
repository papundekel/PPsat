#pragma once
#include "PPsat/variable.hpp"
#include <PPsat/formula.hpp>

#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>

#include <antlr4-runtime.h>

#include <map>

namespace PPsat
{
class variable;

class visitor_DIMACS final : public parser_DIMACSVisitor
{
    formula& f;
    std::map<std::size_t, variable&> renaming_from_input;
    std::size_t name_next;
    bool parse_error;

public:
    visitor_DIMACS(formula& formula) noexcept;

private:
    std::optional<std::size_t> parse_number(
        antlr4::tree::TerminalNode* number_node);

    std::any visitInput(parser_DIMACS::InputContext* context);
    std::any visitClause(parser_DIMACS::ClauseContext* context);
    std::any visitLiteral(parser_DIMACS::LiteralContext* context);
};
}
