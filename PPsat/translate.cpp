#include <PPsat/translate.hpp>

void PPsat::translate(std::istream& input, std::ostream& output)
{
    char buffer[4096] = {};
    input.read(buffer, sizeof(buffer));

    output.write(buffer, input.gcount());

    output <<   "\n"
                "cigi\n";
}
