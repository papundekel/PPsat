#pragma once
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <antlr4-runtime.h>

namespace PPsat_base
{
class error_listener final : public antlr4::ANTLRErrorListener
{
    const logger_subroutine logger_inner;
    bool was_error_syntax;
    bool was_error;

public:
    error_listener(const logger& logger_outer) noexcept;

    bool error_encountered() const noexcept;

private:
    void syntaxError(antlr4::Recognizer*,
                     antlr4::Token*,
                     size_t,
                     size_t,
                     const std::string&,
                     std::exception_ptr) override final;

    void reportAmbiguity(antlr4::Parser*,
                         const antlr4::dfa::DFA&,
                         size_t,
                         size_t,
                         bool,
                         const antlrcpp::BitSet&,
                         antlr4::atn::ATNConfigSet*) override final;

    void reportAttemptingFullContext(antlr4::Parser*,
                                     const antlr4::dfa::DFA&,
                                     size_t,
                                     size_t,
                                     const antlrcpp::BitSet&,
                                     antlr4::atn::ATNConfigSet*) override final;

    void reportContextSensitivity(antlr4::Parser*,
                                  const antlr4::dfa::DFA&,
                                  size_t,
                                  size_t,
                                  size_t,
                                  antlr4::atn::ATNConfigSet*) override final;
};
}
