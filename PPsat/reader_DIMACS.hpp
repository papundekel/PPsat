#pragma once
#include <PPsat/reader.hpp>
#include <PPsat/renaming_map.hpp>

namespace PPsat
{
class reader_DIMACS
    : public reader
    , public factory<renaming>::impl<renaming_map<std::string_view>>
{
public:
    result read(std::istream& input,
                formula& formula,
                renaming& renaming) const override final;
};
}
