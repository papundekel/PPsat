#include <PPsat/preprocessor_basic.hpp>

#include <PPsat-base/ranges_to.hpp>

#include <algorithm>
#include <functional>

std::pair<std::optional<std::vector<PPsat::literal>>, bool>
PPsat::preprocessor_basic::preprocess(
    PPsat_base::view_any<literal> literals) const
{
    auto literals_copy = PPsat_base::ranges_to<std::vector>(literals);

    if (literals_copy.empty())
    {
        return {{}, true};
    }

    std::ranges::sort(literals_copy);
    auto [ub, ue] = std::ranges::unique(literals_copy);
    literals_copy.erase(ub, ue);

    auto i = literals_copy.begin();

    while (true)
    {
        i = std::adjacent_find(i,
                               literals_copy.end(),
                               [](literal a, literal b)
                               {
                                   return &a.get_variable() ==
                                          &b.get_variable();
                               });

        if (i == literals_copy.end())
        {
            break;
        }

        i = literals_copy.erase(i, i + 2);
    }

    if (literals_copy.empty())
    {
        return {{}, false};
    }

    return {std::move(literals_copy), false};
}
