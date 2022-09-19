#include <PPsat/renaming_basic.hpp>

PPsat::variable* PPsat::renaming_basic::contains(
    std::string_view name_input) const
{
    const auto i = storage.find(name_input);

    if (i == storage.end())
    {
        return nullptr;
    }

    return &i->second;
}

void PPsat::renaming_basic::emplace(std::string_view name_input,
                                    variable& variable)
{
    storage.try_emplace(name_input, variable);
}
