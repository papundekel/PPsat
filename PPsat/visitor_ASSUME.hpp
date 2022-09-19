#pragma once
#include <PPsat/assumptions.hpp>
#include <PPsat/renaming.hpp>

#include <PPsat-parser_ASSUME/parser_ASSUME.h>
#include <PPsat-parser_ASSUME/parser_ASSUMEVisitor.h>

#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <antlr4-runtime.h>

namespace PPsat
{
class visitor_ASSUME final : public parser_ASSUMEVisitor
{
    const PPsat_base::logger_subroutine logger;
    assumptions& assumption;
    const renaming& renaming_from_input;

public:
    visitor_ASSUME(const PPsat_base::logger& logger_outer,
                   assumptions& assumption,
                   const renaming& renaming_from_input) noexcept;

private:
    std::any visitInput(parser_ASSUME::InputContext* context);
    std::any visitLiteral(parser_ASSUME::LiteralContext* context);
};
}
