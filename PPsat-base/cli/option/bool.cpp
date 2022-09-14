#include <PPsat-base/cli/option/bool.hpp>

PPsat_base::cli::option::bool_::bool_(bool default_) noexcept
    : value(default_)
    , no_argument(true)
{}

std::size_t PPsat_base::cli::option::bool_::argument_count_min() const noexcept
{
    return 0;
}

std::size_t PPsat_base::cli::option::bool_::argument_count_max() const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::bool_::parse(const logger&,
                                           std::size_t,
                                           std::string_view argument) noexcept
{
    no_argument = false;

    if (argument == "on" || argument == "true" || argument == "yes")
    {
        value = true;
        return true;
    }
    else if (argument == "off" || argument == "false" || argument == "no")
    {
        value = false;
        return true;
    }

    return false;
}

bool PPsat_base::cli::option::bool_::parsed() const
{
    if (is_present() && no_argument)
    {
        return true;
    }

    return value;
}
