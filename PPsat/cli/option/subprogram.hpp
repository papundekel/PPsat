#pragma once
#include <PPsat/subprogram.hpp>

#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>

namespace PPsat::cli::option
{
class subprogram
    : public PPsat_base::cli::option::simple_named_enum_typed<
          PPsat::subprogram::selection>

{
    static int fallback(const PPsat_base::logger&,
                        cli::options&,
                        cli::argument::file_in&,
                        cli::argument::file_out&)
    {
        return 0;
    }

public:
    using simple_named_enum_typed<
        PPsat::subprogram::selection>::simple_named_enum_typed;

    PPsat::subprogram::type& parsed_subprogram() const noexcept
    {
        switch (parsed())
        {
            case PPsat::subprogram::selection::convert:
                return PPsat::subprogram::convert;
            case PPsat::subprogram::selection::solve:
                return PPsat::subprogram::solve;
            default:
                return fallback;
        }
    }
};
}
