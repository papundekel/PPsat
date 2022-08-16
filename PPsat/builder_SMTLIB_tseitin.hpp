#pragma once
#include <PPsat-base/builder.hpp>
#include <PPsat-base/factory.hpp>

namespace PPsat
{
class builder_SMTLIB_tseitin final : public PPsat_base::builder
{
    bool nnf;

public:
    builder_SMTLIB_tseitin(bool nnf) noexcept
        : nnf(nnf)
    {}

private:
    bool read(const PPsat_base::logger& logger_outer,
              std::istream& input,
              PPsat_base::formula& formula) const override final;
};
}
