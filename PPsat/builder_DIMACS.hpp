#pragma once
#include <PPsat-base/builder.hpp>

namespace PPsat
{
class builder_DIMACS final : public PPsat_base::builder
{
    bool read(const PPsat_base::logger& logger_outer,
              std::istream& input,
              PPsat_base::formula& formula) const override final;
};
}
