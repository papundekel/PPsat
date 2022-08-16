#include <PPsat-base/cli/parser.hpp>

decltype(PPsat_base::cli::parser::option_map)::const_iterator
PPsat_base::cli::parser::find(const std::string_view name) const
{
    const auto i = std::ranges::lower_bound(option_map,
                                            name,
                                            std::ranges::less(),
                                            &key_value_pair::first);

    if (i != option_map.end() && i->first == name)
        return i;
    else
        return option_map.end();
}
