#include <PPsat/formula.hpp>

void PPsat::formula::write_DIMACS(
    std::ostream& output,
    std::function<std::ostream&(std::ostream&, const literal&)> transform) const
{
    for_each(
        [&output, &transform](const clause& c)
        {
            c.for_each(
                [&output, &transform](const literal& l)
                {
                    transform(output, l) << " ";
                });
            output << "0\n";
        });
}
