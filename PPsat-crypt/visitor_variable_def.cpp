#include <PPsat-crypt/visitor_variable_def.hpp>

#include <PPsat-base/containers.hpp>
#include <PPsat-base/ranges_to.hpp>
#include <PPsat-base/view_chain.hpp>

#include <algorithm>
#include <ranges>

PPsat_crypt::visitor_variable_def::visitor_variable_def(
    std::size_t base,
    std::ostream& output) noexcept
    : base(base)
    , output(output)
    , variables_defined()
    , atom_i(0)
{}

std::any PPsat_crypt::visitor_variable_def::visitInput(
    parser_CRYPT::InputContext* context)
{
    visit(context->formula());
    output << "\n\n";
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitParentheses(
    parser_CRYPT::ParenthesesContext* context)
{
    visit(context->formula());
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitDisjunction(
    parser_CRYPT::DisjunctionContext* context)
{
    visit(context->formula(0));
    visit(context->formula(1));
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitConjunction(
    parser_CRYPT::ConjunctionContext* context)
{
    visit(context->formula(0));
    visit(context->formula(1));
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitNegation(
    parser_CRYPT::NegationContext* context)
{
    visit(context->formula());
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitAtomFormula(
    parser_CRYPT::AtomFormulaContext* context)
{
    visit(context->atom());
    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitAtom(
    parser_CRYPT::AtomContext* context)
{
    auto words = PPsat_base::ranges_to<PPsat_base::vector>(
        context->WORD() |
        std::views::transform(&antlr4::tree::TerminalNode::getText));

    for (auto word : words)
    {
        for (auto character : word)
        {
            if (!variables_defined.contains(character))
            {
                output << "(declare-const v" << character << " Int)\n"
                       << "(assert (< v" << character << " " << base << "))\n"
                       << "(assert (not (< v" << character << " 0)))\n"
                       << "\n";

                variables_defined.emplace(character);
            }
        }
    }

    const auto length_max = std::ranges::max_element(words,
                                                     std::ranges::less(),
                                                     &std::string::length)
                                ->length();

    for (auto significance_i = 0uz; significance_i != length_max;
         ++significance_i)
    {
        output << "(declare-const x" << atom_i << "_" << significance_i
               << " Int)\n";
        output << "(declare-const y" << atom_i << "_" << significance_i
               << " Int)\n";
    }

    ++atom_i;

    return {};
}

std::any PPsat_crypt::visitor_variable_def::visitOperation(
    parser_CRYPT::OperationContext* context)
{
    return {};
}
