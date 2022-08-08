#pragma once
#include <PPsat/builder.hpp>
#include <PPsat/factory.hpp>
#include <PPsat/renaming_map.hpp>

namespace PPsat
{
class builder_SMTLIB_tseitin
    : public builder
    , public factory<renaming>::impl<renaming_map<std::string>>
{
    bool nnf;

public:
    builder_SMTLIB_tseitin(bool nnf) noexcept
        : nnf(nnf)
    {}

    result read(const logger& logger_outer,
                std::istream& input,
                formula& formula,
                renaming& renaming) const override final;
};
}
