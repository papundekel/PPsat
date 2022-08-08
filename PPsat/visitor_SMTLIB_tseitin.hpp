#include <PPsat/literal_pair.hpp>
#include <PPsat/renaming.hpp>
#include <PPsat/tseitin_builder.hpp>

#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIBVisitor.h>

#include <memory>

namespace PPsat
{
class visitor_SMTLIB_tseitin final : public parser_SMTLIBVisitor
{
    const tseitin_builder& builder;
    std::map<std::string_view, std::size_t> renaming_from_input;
    renaming& renaming_from_native;
    std::size_t name_next;

public:
    visitor_SMTLIB_tseitin(const tseitin_builder& builder,
                           renaming& renaming_from_native) noexcept;

private:
    std::size_t get_next_name() noexcept;

    literal_pair handle_introduced() noexcept;
    literal_pair handle_input(std::string name);

    literal_pair visit_typed(antlr4::tree::ParseTree* tree);

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
