#include <PPsat/preprocessor_id.hpp>

#include <PPsat-base/ranges_to.hpp>

#include <algorithm>

std::pair<std::optional<std::vector<PPsat::literal>>, bool>
PPsat::preprocessor_id::preprocess(PPsat_base::view_any<literal> literals) const
{
    return {PPsat_base::ranges_to<std::vector>(literals), false};
}
