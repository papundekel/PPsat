#include <iostream>

namespace PPexe
{
int main(std::istream& cin,
         std::ostream& cout,
         std::ostream& cerr,
         int argc,
         char** argv);
}

int main(int argc, char** argv)
{
    return PPexe::main(std::cin, std::cout, std::cerr, argc, argv);
}
