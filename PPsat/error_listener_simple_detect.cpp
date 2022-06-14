#include <PPsat/error_listener_simple_detect.hpp>

PPsat::error_listener_simple_detect::error_listener_simple_detect() noexcept
    : was_error(false)
{}

bool PPsat::error_listener_simple_detect::error_encountered() const noexcept
{
    return was_error;
}

void PPsat::error_listener_simple_detect::syntaxError(antlr4::Recognizer*,
                                                      antlr4::Token*,
                                                      size_t,
                                                      size_t,
                                                      const std::string&,
                                                      std::exception_ptr)
{
    was_error = true;
}

void PPsat::error_listener_simple_detect::reportAmbiguity(
    antlr4::Parser*,
    const antlr4::dfa::DFA&,
    size_t,
    size_t,
    bool,
    const antlrcpp::BitSet&,
    antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}

void PPsat::error_listener_simple_detect::reportAttemptingFullContext(
    antlr4::Parser*,
    const antlr4::dfa::DFA&,
    size_t,
    size_t,
    const antlrcpp::BitSet&,
    antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}

void PPsat::error_listener_simple_detect::reportContextSensitivity(
    antlr4::Parser*,
    const antlr4::dfa::DFA&,
    size_t,
    size_t,
    size_t,
    antlr4::atn::ATNConfigSet*)
{
    was_error = true;
}
