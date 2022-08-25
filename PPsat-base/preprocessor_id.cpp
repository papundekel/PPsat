#include <PPsat-base/preprocessor_id.hpp>
#include <PPsat-base/ranges_to.hpp>

#include <algorithm>

std::pair<std::optional<std::vector<PPsat_base::literal>>, bool>
PPsat_base::preprocessor_id::preprocess(view_any<literal> literals) const
{
    return {ranges_to<std::vector>(literals), false};
}
