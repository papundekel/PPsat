#include <PPsat/CNF_simple.hpp>
#include <PPsat/literal_pair.hpp>
#include <PPsat/small_static_storage.hpp>

#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

#include <map>

namespace PPsat
{
class reader_SMTLIB_tseitin final : public parser_SMTLIBVisitor
{
    using name_t = std::size_t;

    using literal = literal_pair<name_t>;

    template <typename T>
    using clause_storage = small_static_storage<T, 3>;

    std::map<std::string, name_t> renaming;
    CNF_simple<std::vector, clause_storage, literal> output_formula;
    literal subformula_literal;
    name_t name_next;
    bool nnf;
    
public:
    reader_SMTLIB_tseitin() noexcept
        : renaming()
        , subformula_literal()
        , output_formula()
        , name_next(0)
    {}

private:
    name_t next_name() noexcept
    {
        return name_next++;
    }

    literal handle_variable_introduced() noexcept
    {
        return {next_name(), true};
    }

    literal handle_variable_input(std::string&& name)
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

    antlrcpp::Any visitConjunction(
        parser_SMTLIB::ConjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = handle_variable_introduced();

        output_formula.push_conjunction(subformula_literal, left, right, nnf);

        return {};
    }

    antlrcpp::Any visitDisjunction(
        parser_SMTLIB::DisjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = handle_variable_introduced();

        output_formula.push_disjunction(subformula_literal, left, right, nnf);

        return {};
    }

    antlrcpp::Any visitNegation(
        parser_SMTLIB::NegationContext* context) override final
    {
        visit(context->formula());
        auto inner = subformula_literal;

        subformula_literal = handle_variable_introduced();

        output_formula.push_negation(subformula_literal, inner, nnf);

        return {};
    }

    antlrcpp::Any visitVariable(
        parser_SMTLIB::VariableContext* context) override final
    {
        auto variable_name = context->VAR()->getSymbol()->getText();

        subformula_literal = handle_variable_input(std::move(variable_name));

        return {};
    }

    antlrcpp::Any visitInput(
        parser_SMTLIB::InputContext* context) override final
    {
        visit(context->formula());

        output_formula.push_literal(subformula_literal);

        return {};
    }

public:
    auto read(parser_SMTLIB::InputContext* context, bool nnf) &&
    {
        this->nnf = nnf;

        visitInput(context);

        return std::make_tuple(std::move(output_formula),
                               std::move(renaming),
                               name_next);
    }
};
}
