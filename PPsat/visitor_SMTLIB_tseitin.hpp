#include <PPsat/literal.hpp>
#include <PPsat/renaming.hpp>
#include <PPsat/tseitin_builder.hpp>
#include <PPsat/variable.hpp>

#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

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
    const PPsat_base::logger_subroutine logger;
    const tseitin_builder& builder;
    renaming& renaming_from_input;
    std::size_t name_next;

public:
    visitor_SMTLIB_tseitin(const PPsat_base::logger& logger_outer,
                           const tseitin_builder& builder,
                           renaming& renaming_from_input) noexcept;

private:
    std::size_t get_next_name() noexcept;
    literal handle_variable(std::string name);
    literal visit_typed(antlr4::tree::ParseTree* tree);

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
