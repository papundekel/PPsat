#include "PPsat-base/literal.hpp"
#include "PPsat-base/logger_subroutine.hpp"
#include "PPsat/assumptions.hpp"
#include <PPsat/visitor_ASSUME.hpp>
#include <any>

PPsat::visitor_ASSUME::visitor_ASSUME(
    const PPsat_base::logger& logger_outer,
    assumptions& assumption,
    const renaming& renaming_from_input) noexcept
    : logger(logger_outer, "visitor-ASSUME")
    , assumption(assumption)
    , renaming_from_input(renaming_from_input)
{}

std::any PPsat::visitor_ASSUME::visitInput(parser_ASSUME::InputContext* context)
{
    for (auto* const literal_context : context->literal())
    {
        const auto literal_any = visit(literal_context);

        if (!literal_any.has_value())
        {
            return {};
        }

        assumption.add(std::any_cast<PPsat_base::literal>(literal_any));
    }

    return nullptr;
}

std::any PPsat::visitor_ASSUME::visitLiteral(
    parser_ASSUME::LiteralContext* context)
{
    const auto name_input = context->VAR()->getText();
    auto variable_opt = renaming_from_input.contains(name_input);

    if (!variable_opt)
    {
        logger << "Invalid variable name \"" << name_input << "\"\n";
        return {};
    }

    return PPsat_base::literal(*variable_opt, context->NEGATED() == nullptr);
}
