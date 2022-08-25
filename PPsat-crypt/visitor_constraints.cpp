#include <PPsat-crypt/visitor_constraints.hpp>

#include <PPsat-base/ranges_to.hpp>
#include <PPsat-base/view_chain.hpp>

#include <algorithm>
#include <ranges>
#include <string>

PPsat_crypt::visitor_constraints::visitor_constraints(
    std::size_t base,
    std::ostream& output) noexcept
    : base(base)
    , output(output)
    , atom_i(0)
{}

std::any PPsat_crypt::visitor_constraints::visitInput(
    parser_CRYPT::InputContext* context)
{
    output << "\n"
           << "(assert\n";

    visit(context->formula());

    output << ")\n"
           << "\n"
           << "(check-sat)\n";

    return {};
}

std::any PPsat_crypt::visitor_constraints::visitParentheses(
    parser_CRYPT::ParenthesesContext* context)
{
    visit(context->formula());
    return {};
}

std::any PPsat_crypt::visitor_constraints::visitDisjunction(
    parser_CRYPT::DisjunctionContext* context)
{
    output << "(or\n";
    visit(context->formula(0));
    output << "\n";
    visit(context->formula(1));
    output << ")\n";
    return {};
}

std::any PPsat_crypt::visitor_constraints::visitConjunction(
    parser_CRYPT::ConjunctionContext* context)
{
    output << "(and\n";
    visit(context->formula(0));
    output << "\n";
    visit(context->formula(1));
    output << ")\n";
    return {};
}

std::any PPsat_crypt::visitor_constraints::visitNegation(
    parser_CRYPT::NegationContext* context)
{
    output << "(not \n";
    visit(context->formula());
    output << ")\n";
    return {};
}

std::any PPsat_crypt::visitor_constraints::visitAtomFormula(
    parser_CRYPT::AtomFormulaContext* context)
{
    visit(context->atom());
    return {};
}

std::any PPsat_crypt::visitor_constraints::visitAtom(
    parser_CRYPT::AtomContext* context)
{
    const auto [operands, result] = [context]()
    {
        const auto get_text = &antlr4::tree::TerminalNode::getText;

        const auto context_words = context->WORD();
        return std::make_pair(PPsat_base::ranges_to<std::vector>(
                                  context_words |
                                  std::views::take(context_words.size() - 1) |
                                  std::views::transform(get_text)),
                              std::invoke(get_text, context_words.back()));
    }();

    const auto operators = PPsat_base::ranges_to<std::vector>(
        context->operation() |
        std::views::transform(
            [this](auto* context_operator)
            {
                return std::any_cast<bool>(visit(context_operator)) ? '+' : '-';
            }));

    output << "; ";

    auto operand_i = operands.begin();
    for (auto operator_i = operators.begin(); operator_i != operators.end();
         ++operator_i, ++operand_i)
    {
        output << *operand_i << *operator_i;
    }
    output << *operand_i << "=" << result << "\n";

    output << "(and\n";

    const auto handle_head = [this](const std::string& word)
    {
        if (word.size() <= 1)
        {
            return;
        }

        const auto character = word[0];
        if (!variable_head.contains(character))
        {
            output << "(not (= 0 v" << character << ")) ";
            variable_head.emplace(character);
        }
    };

    output << "; First digit in a number cannot be 0.\n";
    for (const auto& operand : operands)
    {
        handle_head(operand);
    }
    handle_head(result);
    output << "\n";

    const auto s_max =
        std::ranges::max(std::ranges::max_element(operands,
                                                  std::ranges::less(),
                                                  &std::string::length)
                             ->length(),
                         result.length() + 1);

    const auto operand_has_bit =
        [](const std::string& operand, std::size_t significance_i)
    {
        if (significance_i < operand.size())
        {
            return std::make_pair(true, *(operand.end() - 1 - significance_i));
        }
        else
        {
            return std::make_pair(false, 'x');
        }
    };

    const auto get_operator = [&operators](auto operator_i)
    {
        if (operator_i != operators.rend())
        {
            return *operator_i;
        }
        else
        {
            return '+';
        }
    };

    const auto print_expression =
        [this, &operators, &operands, get_operator, operand_has_bit](
            std::size_t significance_i)
    {
        auto operator_i = operators.rbegin();

        for (auto operand_i = operands.rbegin(); operand_i != operands.rend();
             ++operand_i, ++operator_i)
        {
            const char op = get_operator(operator_i);

            const auto [has_bit, _] =
                operand_has_bit(*operand_i, significance_i);

            if (has_bit)
            {
                output << "(" << op << " ";
            }
        }

        output << "0";

        for (auto operand_i = operands.begin(); operand_i != operands.end();
             ++operand_i)
        {
            const auto [has_bit, bit] =
                operand_has_bit(*operand_i, significance_i);

            if (has_bit)
            {
                output << " v" << bit << ")";
            }
        }
    };

    auto magnitude = base;

    output << "\n"
           << "; Results per significance.\n";

    for (auto s = 0; s != s_max; ++s)
    {
        output << "(= x" << atom_i << "_" << s << " ";
        print_expression(s);
        output << ")\n";
    }

    output << "\n"
           << "; Results with carry values added.\n";

    for (auto s = 0; s != s_max; ++s)
    {
        output << "(= y" << atom_i << "_" << s << " (+ x" << atom_i << "_" << s
               << " ";

        if (s != 0)
        {
            output << "(div y" << atom_i << "_" << s - 1 << " " << base << ")";
        }
        else
        {
            output << "0";
        }

        output << "))\n";
    }

    output << "\n"
           << "; The result characters are mod base the results with carry.\n";
    for (auto i = 0; i != result.length(); ++i)
    {
        output << "(= v" << *(result.end() - 1 - i) << " (mod y" << atom_i
               << "_" << i << " " << base << "))\n";
    }

    output << "\n"
           << "; The result shall be exact.\n"
           << "(= y" << atom_i << "_" << result.length() << " 0)\n";

    output << ")\n";

    ++atom_i;

    return {};
}

std::any PPsat_crypt::visitor_constraints::visitOperation(
    parser_CRYPT::OperationContext* context)
{
    return context->ADD() != nullptr;
}
