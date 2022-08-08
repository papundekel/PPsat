#include <PPsat/visitor_SMTLIB_tseitin.hpp>

#include <PPsat/discard.hpp>
#include <PPsat/literal_pair.hpp>

PPsat::visitor_SMTLIB_tseitin::visitor_SMTLIB_tseitin(
    const tseitin_builder& builder,
    renaming& renaming_from_native) noexcept
    : builder(builder)
    , renaming_from_input()
    , renaming_from_native(renaming_from_native)
    , name_next(0)
{}

std::size_t PPsat::visitor_SMTLIB_tseitin::get_next_name() noexcept
{
    return name_next++;
}

PPsat::literal_pair PPsat::visitor_SMTLIB_tseitin::handle_introduced() noexcept
{
    return {get_next_name(), true};
}

PPsat::literal_pair PPsat::visitor_SMTLIB_tseitin::handle_input(
    std::string name_input)
{
    return {[this, &name_input]()
            {
                if (auto i = renaming_from_input.find(name_input);
                    i != renaming_from_input.end())
                {
                    return i->second;
                }
                else
                {
                    const auto name_native = get_next_name();

                    const auto name_input_view =
                        renaming_from_native.rename(name_native,
                                                    std::move(name_input));
                    renaming_from_input.try_emplace(name_input_view,
                                                    name_native);

                    return name_native;
                }
            }(),
            true};
}

PPsat::literal_pair PPsat::visitor_SMTLIB_tseitin::visit_typed(
    antlr4::tree::ParseTree* tree)
{
    return std::any_cast<literal_pair>(visit(tree));
}

std::any PPsat::visitor_SMTLIB_tseitin::visitConjunction(
    parser_SMTLIB::ConjunctionContext* context)
{
    const auto left = visit_typed(context->formula(0));
    const auto right = visit_typed(context->formula(1));

    const auto subformula_literal = handle_introduced();

    builder.push_conjunction(subformula_literal, left, right);

    return subformula_literal;
}

std::any PPsat::visitor_SMTLIB_tseitin::visitDisjunction(
    parser_SMTLIB::DisjunctionContext* context)
{
    const auto left = visit_typed(context->formula(0));
    const auto right = visit_typed(context->formula(1));

    const auto subformula_literal = handle_introduced();

    builder.push_disjunction(subformula_literal, left, right);

    return subformula_literal;
}

std::any PPsat::visitor_SMTLIB_tseitin::visitNegation(
    parser_SMTLIB::NegationContext* context)
{
    const auto inner = visit_typed(context->formula());

    const auto subformula_literal = handle_introduced();

    builder.push_negation(subformula_literal, inner);

    return subformula_literal;
}

std::any PPsat::visitor_SMTLIB_tseitin::visitVariable(
    parser_SMTLIB::VariableContext* context)
{
    return handle_input(context->VAR()->getSymbol()->getText());
}

std::any PPsat::visitor_SMTLIB_tseitin::visitInput(
    parser_SMTLIB::InputContext* context)
{
    builder.push_literal(visit_typed(context->formula()));

    return name_next;
}
