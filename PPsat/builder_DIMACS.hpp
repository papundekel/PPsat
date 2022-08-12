#pragma once
#include <PPsat/builder.hpp>

namespace PPsat
{
class builder_DIMACS final : public builder
{
public:
    result read(const logger& logger_outer,
                std::istream& input,
                formula& formula) const override final;
};
}
