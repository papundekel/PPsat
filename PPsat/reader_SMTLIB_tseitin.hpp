#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

namespace PPsat
{
template <template <typename> typename Literal>
struct template_t
{};

template <template <typename> typename Literal,
          typename Renaming,
          typename FormulaBuilder>
class reader_SMTLIB_tseitin final : public parser_SMTLIBVisitor
{
    using name_t = Renaming::mapped_type;
    using literal_t = Literal<name_t>;

    Renaming& renaming;
    FormulaBuilder& formula_builder;
    literal_t subformula_literal;
    name_t name_next;

public:
    reader_SMTLIB_tseitin(template_t<Literal>,
                          Renaming& renaming,
                          FormulaBuilder& formula_builder) noexcept
        : renaming(renaming)
        , formula_builder(formula_builder)
        , subformula_literal()
        , name_next(0)
    {}

private:
    name_t next_name() noexcept
    {
        return name_next++;
    }

    literal_t handle_variable_introduced() noexcept
    {
        return {next_name(), true};
    }

    literal_t handle_variable_input(std::string name)
    {
        name_t variable_p;

        if (auto i = renaming.find(name); i != renaming.end())
        {
            variable_p = i->second;
        }
        else
        {
            variable_p = next_name();

            renaming.try_emplace(std::move(name), variable_p);
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

        formula_builder.push_conjunction(subformula_literal, left, right);

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

        formula_builder.push_disjunction(subformula_literal, left, right);

        return {};
    }

    std::any visitNegation(
        parser_SMTLIB::NegationContext* context) override final
    {
        visit(context->formula());
        const auto inner = subformula_literal;

        subformula_literal = handle_variable_introduced();

        formula_builder.push_negation(subformula_literal, inner);

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

        formula_builder.push_literal(subformula_literal);

        return {};
    }

public:
    auto read(parser_SMTLIB::InputContext* context) &&
    {
        visitInput(context);

        return name_next;
    }
};
}
