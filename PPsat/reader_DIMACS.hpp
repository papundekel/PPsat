#include <PPsat/CNF_simple.hpp>

#include <PPsat-parser_DIMACS/parser_DIMACSVisitor.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>

#include <list>
#include <map>

namespace
{
auto greater_power10(auto x)
{
    decltype(x) power = 1;

    while (power <= x)
    {
        power *= 10;
    }

    return power;
}
}

namespace PPsat
{
class reader_DIMACS final : public parser_DIMACSVisitor
{
    using variable_name_t = unsigned int;

    class variable_t
    {
    public:
        mutable variable_name_t name;
    };

    using literal = literal_pair<variable_t*>;

    std::list<variable_t> variables_input;
    std::list<variable_t> variables_introduced;
    std::map<std::string, variable_t*> input_variable_name_mapping;
    literal subformula_literal;
    CNF_simple<std::vector, literal> output_formula;

public:
    reader_SMTLIB_tseitin() noexcept
        : variables_input()
        , variables_introduced()
        , input_variable_name_mapping()
        , subformula_literal()
        , output_formula()
    {}

private:
    variable_t* make_next_variable(auto& variables) noexcept
    {
        variables.push_back({(variable_name_t)variables.size() + 1});
        return &variables.back();
    }

    variable_t* handle_input_variable(std::string&& name)
    {
        variable_t* variable_p;

        if (auto i = input_variable_name_mapping.find(name);
            i != input_variable_name_mapping.end())
        {
            variable_p = i->second;
        }
        else
        {
            variable_p = make_next_variable(variables_input);

            input_variable_name_mapping.try_emplace(std::move(name),
                                                    variable_p);
        }

        return variable_p;
    }

public:
    auto result() noexcept
    {
        return std::move(output_formula);
    }

    void write(std::ostream& output) const
    {
        auto introduced_new_offset = greater_power10(variables_input.size());

        for (auto i = introduced_new_offset + 1;
             auto& variable : variables_introduced)
        {
            variable.name = (variable_name_t)i;
            ++i;
        }

        for (const auto& [name_input, variable] : input_variable_name_mapping)
        {
            output << "c " << variable->name << "=" << name_input << "\n";
        }

        output << "c The introduced variables are named with an offset of "
               << introduced_new_offset << "\n"
               << "p cnf "
               << variables_input.size() + variables_introduced.size() << " "
               << output_formula.clauses_count() << "\n";

        output_formula.write_DIMACS(output);
    }

    antlrcpp::Any visitInput(parser_SMTLIB::InputContext* context) override final
    {
        visit(context->formula());

        output_formula.push_literal(subformula_literal);

        return {};
    }

    antlrcpp::Any visitConjunction(
        parser_SMTLIB::ConjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = {make_next_variable(variables_introduced), true};

        output_formula.push_conjunction(subformula_literal, left, right);

        return {};
    }

    antlrcpp::Any visitDisjunction(
        parser_SMTLIB::DisjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = {make_next_variable(variables_introduced), true};

        output_formula.push_disjunction(subformula_literal, left, right);

        return {};
    }

    antlrcpp::Any visitNegation(
        parser_SMTLIB::NegationContext* context) override final
    {
        auto variable_name = context->VAR()->getSymbol()->getText();

        subformula_literal = {handle_input_variable(std::move(variable_name)),
                              false};

        return {};
    }

    antlrcpp::Any visitVariable(
        parser_SMTLIB::VariableContext* context) override final
    {
        auto variable_name = context->VAR()->getSymbol()->getText();

        subformula_literal = {handle_input_variable(std::move(variable_name)),
                              true};

        return {};
    }
};
}
