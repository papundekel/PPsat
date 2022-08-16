#include <PPsat-crypt-parser_CRYPT/parser_CRYPT.h>
#include <PPsat-crypt-parser_CRYPT/parser_CRYPTVisitor.h>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

#include <cstddef>
#include <functional>
#include <iosfwd>
#include <vector>

namespace PPsat_crypt
{
class visitor_variable_def final : public parser_CRYPTVisitor
{
    std::size_t base;
    std::ostream& output;
    std::set<char> variables_defined;
    std::size_t atom_i;

public:
    visitor_variable_def(std::size_t base, std::ostream& output) noexcept;

private:
    std::any visitInput(parser_CRYPT::InputContext* context) override final;
    std::any visitDisjunction(
        parser_CRYPT::DisjunctionContext* context) override final;
    std::any visitNegation(
        parser_CRYPT::NegationContext* context) override final;
    std::any visitConjunction(
        parser_CRYPT::ConjunctionContext* context) override final;
    std::any visitAtomFormula(
        parser_CRYPT::AtomFormulaContext* context) override final;
    std::any visitParentheses(
        parser_CRYPT::ParenthesesContext* context) override final;
    std::any visitAtom(parser_CRYPT::AtomContext* context) override final;
    std::any visitOperation(
        parser_CRYPT::OperationContext* context) override final;
};
}
