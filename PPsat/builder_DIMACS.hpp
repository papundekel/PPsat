#pragma once
#include <PPsat/builder.hpp>
#include <PPsat/renaming_map.hpp>

namespace PPsat
{
class builder_DIMACS final
    : public builder
    , public factory<renaming>::impl<renaming_map<std::size_t>>
{
public:
    result read(const logger& logger_outer,
                std::istream& input,
                formula& formula,
                renaming& renaming) const override final;
};
}
