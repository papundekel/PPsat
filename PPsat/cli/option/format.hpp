#pragma once
#include <PPsat/cli/option/named.hpp>
#include <PPsat/cli/parameter/simple.hpp>
#include <PPsat/formula_format.hpp>

namespace PPsat::cli::option
{
class format : public named<parameter::simple<option_>, "format">
{
    formula_format format;

public:
    std::size_t argument_count() const noexcept;

    bool parse(std::size_t i, std::string_view argument_format) noexcept;

    formula_format parsed_format() const noexcept;
};
}
