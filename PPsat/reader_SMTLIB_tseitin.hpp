#pragma once
#include <PPsat/factory.hpp>
#include <PPsat/reader.hpp>
#include <PPsat/renaming_map.hpp>

namespace PPsat
{
class reader_SMTLIB_tseitin
    : public reader
    , public factory<renaming>::impl<renaming_map<std::string_view>>
{
    bool nnf;

public:
    reader_SMTLIB_tseitin(bool nnf) noexcept
        : nnf(nnf)
    {}

    result read(std::istream& input,
                formula& formula,
                renaming& renaming) const override final;
};
}
