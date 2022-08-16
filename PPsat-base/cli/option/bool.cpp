#include <PPsat-base/cli/option/bool.hpp>

std::size_t PPsat_base::cli::option::bool_::argument_count() const noexcept
{
    return 0;
}

bool PPsat_base::cli::option::bool_::parse(std::size_t,
                                           std::string_view) noexcept
{
    return true;
}
