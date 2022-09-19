#pragma once
#include <PPsat/renaming.hpp>
#include <PPsat/renaming_int.hpp>

#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>

#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <antlr4-runtime.h>

#include <map>
#include <optional>

namespace PPsat
{
class variable;
class formula;

class visitor_DIMACS final : public parser_DIMACSVisitor
{
    const PPsat_base::logger_subroutine logger;
    formula& formula_;
    renaming_int& renaming_from_input;
    std::size_t name_next;
    bool parse_error;

public:
    visitor_DIMACS(const PPsat_base::logger& logger_outer,
                   formula& formula,
                   renaming_int& renaming_from_input) noexcept;

private:
    std::any visitInput(parser_DIMACS::InputContext* context) override final;
    std::any visitClause(parser_DIMACS::ClauseContext* context) override final;
    std::any visitLiteral(
        parser_DIMACS::LiteralContext* context) override final;
};
}
