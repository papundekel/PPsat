#pragma once
#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>

#include <antlr4-runtime.h>

#include <map>
#include <optional>

namespace PPsat_base
{
class variable;
class formula;
}

namespace PPsat
{

class visitor_DIMACS final : public parser_DIMACSVisitor
{
    PPsat_base::formula& f;
    std::map<std::size_t, PPsat_base::variable&> renaming_from_input;
    std::size_t name_next;
    bool parse_error;

public:
    visitor_DIMACS(PPsat_base::formula& formula) noexcept;

private:
    std::optional<std::size_t> parse_number(
        antlr4::tree::TerminalNode* number_node);

    std::any visitInput(parser_DIMACS::InputContext* context);
    std::any visitClause(parser_DIMACS::ClauseContext* context);
    std::any visitLiteral(parser_DIMACS::LiteralContext* context);
};
}
