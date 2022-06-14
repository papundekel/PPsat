#pragma once
#include <iosfwd>

namespace PPsat
{
    enum class error_code
    {
        none,
        syntax,
    };

    error_code tseitin_translate(std::istream& input, std::ostream& output, bool nnf);
}
