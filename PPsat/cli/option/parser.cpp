#include <PPsat/cli/option/parser.hpp>

decltype(PPsat::cli::option::parser::option_map)::const_iterator
PPsat::cli::option::parser::find(std::string_view name) const
{
    const auto i = std::ranges::lower_bound(option_map,
                                            name,
                                            std::ranges::less{},
                                            &key_value_pair::first);

    if (i->first == name)
        return i;
    else
        return option_map.end();
}
