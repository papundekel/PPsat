#include "PPsat-base/logger.hpp"
#include <PPsat/visitor_SMTLIB_tseitin.hpp>

#include <PPsat-base/discard.hpp>
#include <PPsat-base/variable.hpp>

PPsat::visitor_SMTLIB_tseitin::visitor_SMTLIB_tseitin(
    const PPsat_base::logger& logger_outer,
    const PPsat_base::tseitin_builder& builder,
    renaming& renaming_from_input) noexcept
    : logger(logger_outer, "visitor-SMTLIB")
    , builder(builder)
    , renaming_from_input(renaming_from_input)
{}

PPsat_base::literal PPsat::visitor_SMTLIB_tseitin::handle_variable(
    std::string name_input)
{
    return {[this, &name_input]() -> PPsat_base::variable&
            {
                const auto variable_opt =
                    renaming_from_input.contains(name_input);

                if (!variable_opt)
                {
                    auto& variable_new = builder.create_new_variable();

                    const auto name_input_view =
                        variable_new.representation_set(std::move(name_input));

                    renaming_from_input.emplace(name_input_view, variable_new);

                    return variable_new;
                }

                return *variable_opt;
            }(),
            true};
}

PPsat_base::literal PPsat::visitor_SMTLIB_tseitin::visit_typed(
    antlr4::tree::ParseTree* tree)
{
    return std::any_cast<PPsat_base::literal>(visit(tree));
}

std::any PPsat::visitor_SMTLIB_tseitin::visitConjunction(
    parser_SMTLIB::ConjunctionContext* context)
{
    const auto left = visit_typed(context->formula(0));
    const auto right = visit_typed(context->formula(1));

    return builder.push_conjunction(left, right);
}

std::any PPsat::visitor_SMTLIB_tseitin::visitDisjunction(
    parser_SMTLIB::DisjunctionContext* context)
{
    const auto left = visit_typed(context->formula(0));
    const auto right = visit_typed(context->formula(1));

    return builder.push_disjunction(left, right);
}

std::any PPsat::visitor_SMTLIB_tseitin::visitNegation(
    parser_SMTLIB::NegationContext* context)
{
    return builder.push_negation(visit_typed(context->formula()));
}

std::any PPsat::visitor_SMTLIB_tseitin::visitVariable(
    parser_SMTLIB::VariableContext* context)
{
    return handle_variable(context->VAR()->getSymbol()->getText());
}

std::any PPsat::visitor_SMTLIB_tseitin::visitInput(
    parser_SMTLIB::InputContext* context)
{
    builder.push_literal(visit_typed(context->formula()));

    return nullptr;
}
