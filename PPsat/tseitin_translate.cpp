#include <PPsat/CNF_simple.hpp>
#include <PPsat/tseitin_translate.hpp>

#include <NNF_lexer.h>
#include <NNF_parserVisitor.h>
#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace PPsat
{
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

class tseitin_translator final : public NNF_parserVisitor
{
    using variable_name_t = unsigned int;

    class variable_t
    {
    public:
        mutable variable_name_t name;
    };

    class literal
    {
    public:
        variable_t* variable;
        bool negated;

        friend std::ostream& operator<<(std::ostream& out, literal l)
        {
            if (l.negated)
            {
                out << "-";
            }
            out << l.variable->name;

            return out;
        }

        literal operator-() const noexcept
        {
            return {variable, !negated};
        }
    };

    std::list<variable_t> variables_input;
    std::list<variable_t> variables_introduced;
    CNF_simple<std::vector, literal> output_formula;
    std::map<std::string, variable_t*> input_variable_name_mapping;
    literal subformula_literal;

public:
    tseitin_translator() noexcept
        : variables_input()
        , variables_introduced()
        , output_formula()
        , input_variable_name_mapping()
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
    void write(std::ostream& output) const
    {
        for (const auto& [name_input, variable] : input_variable_name_mapping)
        {
            output << "c " << variable->name << "=" << name_input << "\n";
        }

        output << "p cnf "
               << variables_input.size() + variables_introduced.size() << " "
               << output_formula.clauses_count() << "\n";

        for (auto introduced_new_begin =
                 greater_power10(variables_input.size()) + 1;
             auto& var : variables_introduced)
        {
            var.name = (variable_name_t)introduced_new_begin;
            ++introduced_new_begin;
        }

        output_formula.write_DIMACS(output);
    }

    antlrcpp::Any visitInput(NNF_parser::InputContext* context) override final
    {
        visit(context->formula());

        output_formula.push_literal(subformula_literal);

        return antlrcpp::Any();
    }

    antlrcpp::Any visitConjunction(
        NNF_parser::ConjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = {make_next_variable(variables_introduced), false};

        output_formula.push_conjunction(subformula_literal, left, right);

        return antlrcpp::Any();
    }

    antlrcpp::Any visitDisjunction(
        NNF_parser::DisjunctionContext* context) override final
    {
        visit(context->formula(0));
        auto left = subformula_literal;
        visit(context->formula(1));
        auto right = subformula_literal;

        subformula_literal = {make_next_variable(variables_introduced), false};

        output_formula.push_disjunction(subformula_literal, left, right);

        return antlrcpp::Any();
    }

    antlrcpp::Any visitNegation(
        NNF_parser::NegationContext* context) override final
    {
        auto variable_name = context->VAR()->getSymbol()->getText();

        subformula_literal = {handle_input_variable(std::move(variable_name)),
                              true};

        return antlrcpp::Any();
    }

    antlrcpp::Any visitVariable(
        NNF_parser::VariableContext* context) override final
    {
        auto variable_name = context->VAR()->getSymbol()->getText();

        subformula_literal = {handle_input_variable(std::move(variable_name)),
                              false};

        return antlrcpp::Any();
    }
};
}

PPsat::error_code PPsat::tseitin_translate(std::istream& input,
                                           std::ostream& output)
{
    antlr4::ANTLRInputStream input_antlr(input);
    NNF_lexer lexer(&input_antlr);
    antlr4::CommonTokenStream tokens(&lexer);
    NNF_parser parser(&tokens);

    auto* input_parsed = parser.input();

    tseitin_translator translator;
    translator.visitInput(input_parsed);
    translator.write(output);

    return error_code::none;
}
