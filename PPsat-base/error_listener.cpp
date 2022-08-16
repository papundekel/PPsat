#include <PPsat-base/error_listener.hpp>

PPsat_base::error_listener::error_listener(const logger& logger_outer) noexcept
    : logger_inner(logger_outer, "parsing")
    , was_error(false)
{}

bool PPsat_base::error_listener::error_encountered() const noexcept
{
    return was_error;
}

void PPsat_base::error_listener::syntaxError(antlr4::Recognizer*,
                                             antlr4::Token*,
                                             size_t l,
                                             size_t c,
                                             const std::string&,
                                             std::exception_ptr)
{
    if (!was_error_syntax)
    {
        logger_inner << "Syntax error at line: " << l << ", column: " << c
                     << ".\n";
    }

    was_error_syntax = true;
    was_error = true;
}

void PPsat_base::error_listener::reportAmbiguity(antlr4::Parser*,
                                                 const antlr4::dfa::DFA&,
                                                 size_t,
                                                 size_t,
                                                 bool,
                                                 const antlrcpp::BitSet&,
                                                 antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}

void PPsat_base::error_listener::reportAttemptingFullContext(
    antlr4::Parser*,
    const antlr4::dfa::DFA&,
    size_t,
    size_t,
    const antlrcpp::BitSet&,
    antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}

void PPsat_base::error_listener::reportContextSensitivity(
    antlr4::Parser*,
    const antlr4::dfa::DFA&,
    size_t,
    size_t,
    size_t,
    antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}
