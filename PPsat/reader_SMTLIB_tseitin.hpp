#include <PPsat/discard.hpp>
#include <PPsat/renaming.hpp>
#include <PPsat/tseitin_builder.hpp>

#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

namespace PPsat
{
class reader_SMTLIB_tseitin final : public parser_SMTLIBVisitor
{
    std::map<std::string, std::size_t> renaming_internal;
    renaming& renaming_external;
    const tseitin_builder& builder;
    literal_pair subformula_literal;
    std::size_t name_next;

public:
    reader_SMTLIB_tseitin(renaming& renaming_external,
                          const tseitin_builder& builder,
                          antlr4::ParserRuleContext* tree) noexcept
        : renaming_internal()
        , renaming_external(renaming_external)
        , builder(builder)
        , subformula_literal()
        , name_next(0)
    {
        visit(tree);
    }

private:
    std::size_t next_name() noexcept
    {
        return name_next++;
    }

    literal_pair handle_variable_introduced() noexcept
    {
        return {next_name(), true};
    }

    literal_pair handle_variable_input(std::string name)
    {
        std::size_t variable_p;

        if (auto i = renaming_internal.find(name); i != renaming_internal.end())
        {
            variable_p = i->second;
        }
        else
        {
            variable_p = next_name();

            std::tie(i, discard) =
                renaming_internal.try_emplace(std::move(name), variable_p);
            renaming_external.rename(i->first, variable_p);
        }

        return {variable_p, true};
    }

    std::any visitConjunction(
        parser_SMTLIB::ConjunctionContext* context) override final
    {
        visit(context->formula(0));
        const auto left = subformula_literal;

        visit(context->formula(1));
        const auto right = subformula_literal;

        subformula_literal = handle_variable_introduced();

        builder.push_conjunction(subformula_literal, left, right);

        return {};
    }

    std::any visitDisjunction(
        parser_SMTLIB::DisjunctionContext* context) override final
    {
        visit(context->formula(0));
        const auto left = subformula_literal;

        visit(context->formula(1));
        const auto right = subformula_literal;

        subformula_literal = handle_variable_introduced();

        builder.push_disjunction(subformula_literal, left, right);

        return {};
    }

    std::any visitNegation(
        parser_SMTLIB::NegationContext* context) override final
    {
        visit(context->formula());
        const auto inner = subformula_literal;

        subformula_literal = handle_variable_introduced();

        builder.push_negation(subformula_literal, inner);

        return {};
    }

    std::any visitVariable(
        parser_SMTLIB::VariableContext* context) override final
    {
        subformula_literal =
            handle_variable_input(context->VAR()->getSymbol()->getText());

        return {};
    }

    std::any visitInput(parser_SMTLIB::InputContext* context) override final
    {
        visit(context->formula());

        builder.push_literal(subformula_literal);

        return {};
    }

public:
    auto result() &&
    {
        return name_next;
    }
};
}
