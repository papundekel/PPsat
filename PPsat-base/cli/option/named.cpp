#include <PPsat-base/cli/option/named.hpp>

PPsat_base::cli::option::named::named(std::string_view name) noexcept
    : name_view(name)
{}

std::string_view PPsat_base::cli::option::named::name() noexcept
{
    return name_view;
}
