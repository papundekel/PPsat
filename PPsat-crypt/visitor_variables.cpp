#include <PPsat-crypt/visitor_variables.hpp>

#include <PPsat-base/containers.hpp>
#include <PPsat-base/ranges_to.hpp>
#include <PPsat-base/view_chain.hpp>

#include <algorithm>
#include <ranges>

PPsat_crypt::visitor_variables::visitor_variables(std::size_t base,
                                                  bool unique,
                                                  std::ostream& output) noexcept
    : base(base)
    , output(output)
    , variables_defined()
    , atom_i(0)
    , unique(unique)
{}

std::any PPsat_crypt::visitor_variables::visitInput(
    parser_CRYPT::InputContext* context)
{
    output << "(set-logic QF_LIA)\n";

    visit(context->formula());

    if (!unique)
    {
        return {};
    }

    output << "\n";
    output << "; Uniqueness.\n";

    output << "(assert (and\n";
    for (const auto a : variables_defined)
    {
        for (const auto b : variables_defined)
        {
            if (a == b)
            {
                continue;
            }

            output << "(not (= v" << a << " v" << b << ")) ";
        }
        output << "\n";
    }
    output << "))\n";

    return {};
}

std::any PPsat_crypt::visitor_variables::visitParentheses(
    parser_CRYPT::ParenthesesContext* context)
{
    visit(context->formula());
    return {};
}

std::any PPsat_crypt::visitor_variables::visitDisjunction(
    parser_CRYPT::DisjunctionContext* context)
{
    visit(context->formula(0));
    visit(context->formula(1));
    return {};
}

std::any PPsat_crypt::visitor_variables::visitConjunction(
    parser_CRYPT::ConjunctionContext* context)
{
    visit(context->formula(0));
    visit(context->formula(1));
    return {};
}

std::any PPsat_crypt::visitor_variables::visitNegation(
    parser_CRYPT::NegationContext* context)
{
    visit(context->formula());
    return {};
}

std::any PPsat_crypt::visitor_variables::visitAtomFormula(
    parser_CRYPT::AtomFormulaContext* context)
{
    visit(context->atom());
    return {};
}

std::any PPsat_crypt::visitor_variables::visitAtom(
    parser_CRYPT::AtomContext* context)
{
    auto words = PPsat_base::ranges_to<PPsat_base::vector>(
        context->WORD() |
        std::views::transform(&antlr4::tree::TerminalNode::getText));

    output << "\n"
           << "; Variables representing characters from input.\n";
    for (auto word : words)
    {
        for (auto character : word)
        {
            if (!variables_defined.contains(character))
            {
                output << "(declare-const v" << character << " Int) "
                       << "(assert (and (< v" << character << " " << base
                       << ") "
                       << "(>= v" << character << " 0)))\n";

                variables_defined.emplace(character);
            }
        }
    }

    const auto s_max =
        std::ranges::max(std::ranges::max_element(words,
                                                  std::ranges::less(),
                                                  &std::string::length)
                             ->length(),
                         words.back().length() + 1);

    output << "\n"
           << "; Helper variables representing "
              "carry values for arithmetics.\n";

    for (auto s = 0uz; s != s_max; ++s)
    {
        output << "(declare-const y" << atom_i << "_" << s << " Int) "
               << "(declare-const x" << atom_i << "_" << s << " Int)\n";
    }

    ++atom_i;

    return {};
}

std::any PPsat_crypt::visitor_variables::visitOperation(
    parser_CRYPT::OperationContext* context)
{
    return {};
}

void PPsat_crypt::visitor_variables::print_get_value() const
{
    output << "(get-value (";
    for (const auto character : variables_defined)
    {
        output << "v" << character << " ";
    }
    output << "))\n";
}
