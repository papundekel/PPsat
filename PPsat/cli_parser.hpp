#pragma once
#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

namespace PPsat
{
class cli_parser
{
    struct option
    {
        bool* present;
        std::vector<std::reference_wrapper<std::string_view>> variables;

        option(bool& present, const std::ranges::range auto& variables)
            : present(&present)
            , variables(std::begin(variables), std::end(variables))
        {}
    };

    using key_value_pair = std::pair<std::string_view, option>;

    std::vector<key_value_pair> option_map;

    auto find(std::string_view name)
    {
        auto i = std::ranges::lower_bound(option_map,
                                          name,
                                          std::ranges::less{},
                                          &key_value_pair::first);

        if (i->first == name)
            return i;
        else
            return option_map.end();
    }

public:
    cli_parser(auto&... options)
    {
        (option_map.emplace_back(
             std::piecewise_construct,
             std::forward_as_tuple(options.name()),
             std::forward_as_tuple(options.presence(), options.variables())),
         ...);

        std::ranges::sort(option_map,
                          [](const auto& a, const auto& b)
                          {
                              return a.first < b.first;
                          });
    }

    void parse(int argc, char** argv, auto argument_oi, auto unrecognised_oi)
    {
        for (auto& [name, o] : option_map)
            *o.present = false;

        auto end = argv + argc;

        for (char** i = argv + 1; i != end;)
        {
            char* cl_argument = *i;

            if (cl_argument[0] == '-')
            {
                auto option = cl_argument + 1;

                auto var_i = find(option);

                if (var_i != option_map.end())
                {
                    auto& o = var_i->second;

                    *o.present = true;

                    i = std::ranges::copy(
                            std::ranges::subrange(i + 1, end) |
                                std::ranges::views::take(o.variables.size()),
                            std::begin(
                                o.variables | std::ranges::views::transform([
                                ](auto& r) -> auto& { return r.get(); })))
                            .in;
                }
                else
                {
                    *unrecognised_oi++ = option;
                    ++i;
                }
            }
            else
            {
                *argument_oi++ = cl_argument;
                ++i;
            }
        }
    }
};
}
