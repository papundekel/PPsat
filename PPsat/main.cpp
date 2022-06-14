#include <PPsat/tseitin_translate.hpp>

#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

int main(int argc, char** argv)
{
    auto argument_count = argc - 1;
    auto arguments = argv + 1;

    if (argument_count > 2)
    {
        std::cerr << "Invalid argument count " << argument_count
                  << ", quitting.\n";
        return 1;
    }

    std::optional<std::ifstream> input_file;
    if (argument_count >= 1)
    {
        input_file.emplace(arguments[0]);
    }
    std::istream& input = input_file ? *input_file : std::cin;

    std::optional<std::ofstream> output_file;
    if (argument_count >= 2)
    {
        output_file.emplace(arguments[1]);
    }
    std::ostream& output = output_file ? *output_file : std::cout;

    auto error = PPsat::tseitin_translate(input, output, false);

    if (error == PPsat::error_code::syntax)
    {
        std::cerr << "Parse error encountered, quitting.\n";
        return 2;
    }

    return 0;
}
