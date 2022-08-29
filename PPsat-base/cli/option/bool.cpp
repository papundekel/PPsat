#include <PPsat-base/cli/option/bool.hpp>

PPsat_base::cli::option::bool_::bool_() noexcept
    : value(false)
    , had_argument(false)
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
    if (argument == "on" || argument == "true" || argument == "yes")
    {
        had_argument = true;
        value = true;
        return true;
    }
    else if (argument == "off" || argument == "false" || argument == "no")
    {
        had_argument = true;
        value = false;
        return true;
    }

    return false;
}

bool PPsat_base::cli::option::bool_::parsed() const
{
    if (had_argument)
    {
        return value;
    }

    return is_present();
}
