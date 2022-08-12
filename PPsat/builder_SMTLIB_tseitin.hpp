#pragma once
#include <PPsat/builder.hpp>
#include <PPsat/factory.hpp>

namespace PPsat
{
class builder_SMTLIB_tseitin final : public builder
{
    bool nnf;

public:
    builder_SMTLIB_tseitin(bool nnf) noexcept
        : nnf(nnf)
    {}

    result read(const logger& logger_outer,
                std::istream& input,
                formula& formula) const override final;
};
}
