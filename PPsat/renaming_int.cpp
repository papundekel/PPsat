#include <PPsat/renaming_int.hpp>

#include <charconv>

std::optional<std::size_t> PPsat::renaming_int::parse_number(
    std::string_view name_input)
{
    std::size_t number;

    const auto result = std::from_chars(name_input.data(),
                                        name_input.data() + name_input.size(),
                                        number);

    if (result.ec != std::errc())
    {
        return {};
    }

    return number;
}

PPsat_base::variable* PPsat::renaming_int::contains(
    std::string_view name_input) const
{
    const auto parsed_opt = parse_number(name_input);

    if (!parsed_opt)
    {
        return {};
    }

    return contains(*parsed_opt);
}

void PPsat::renaming_int::emplace(std::string_view name_input,
                                  PPsat_base::variable& variable)
{
    const auto parsed_opt = parse_number(name_input);

    if (!parsed_opt)
    {
        return;
    }

    emplace(*parsed_opt, variable);
}
