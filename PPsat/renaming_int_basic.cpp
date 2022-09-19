#include <PPsat/renaming_int_basic.hpp>

PPsat::variable* PPsat::renaming_int_basic::contains(
    std::size_t name_input) const
{
    const auto i = storage.find(name_input);

    if (i == storage.end())
    {
        return nullptr;
    }

    return &i->second;
}

void PPsat::renaming_int_basic::emplace(std::size_t name_input,
                                        variable& variable)
{
    storage.try_emplace(name_input, variable);
}
