#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

#include <PPsat-base/literal.hpp>
#include <PPsat-base/tseitin_builder.hpp>
#include <PPsat-base/variable.hpp>

#include <functional>
#include <memory>

namespace PPsat_base
{
class variable;
}

namespace PPsat
{
class visitor_SMTLIB_tseitin final : public parser_SMTLIBVisitor
{
    const PPsat_base::tseitin_builder& builder;
    std::map<std::string_view, PPsat_base::variable&> renaming_from_input;
    std::size_t name_next;

public:
    visitor_SMTLIB_tseitin(const PPsat_base::tseitin_builder& builder) noexcept;

private:
    std::size_t get_next_name() noexcept;

    PPsat_base::literal handle_input(std::string name);

    PPsat_base::literal visit_typed(antlr4::tree::ParseTree* tree);

    std::any visitConjunction(
        parser_SMTLIB::ConjunctionContext* context) override final;
    std::any visitDisjunction(
        parser_SMTLIB::DisjunctionContext* context) override final;
    std::any visitNegation(
        parser_SMTLIB::NegationContext* context) override final;
    std::any visitVariable(
        parser_SMTLIB::VariableContext* context) override final;
    std::any visitInput(parser_SMTLIB::InputContext* context) override final;
};
}
