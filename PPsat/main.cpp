#include <PPsat/cli_parser.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/tseitin_translate.hpp>

#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <utility>
#include <variant>

namespace PPsat::cli_options
{
using namespace std::literals;

template <size_t count>
struct constant_string
{
    char chars[count];

    constexpr constant_string(const char (&string)[count + 1]) noexcept
    {
        std::ranges::copy(string | std::ranges::views::take(count), chars);
    }

    constexpr operator std::string_view() const
    {
        return std::string_view(chars, count);
    }
};
template <size_t count>
constant_string(const char (&)[count]) -> constant_string<count - 1>;

template <constant_string Name>
class bool_option
{
    bool present;

public:
    bool_option() = default;

    static constexpr std::string_view name() noexcept
    {
        return Name;
    }

    bool& presence() noexcept
    {
        return present;
    }

    static constexpr auto variables() noexcept
    {
        return std::ranges::views::empty<std::string_view>;
    }

    operator bool() const noexcept
    {
        return present;
    }
};
}

int main(int argc, char** argv)
{
    PPsat::cli_options::bool_option<"convert"> option_convert;
    PPsat::cli_options::bool_option<"dpll"> option_dpll;
    PPsat::cli_options::bool_option<"cdcl"> option_cdcl;
    PPsat::cli_options::bool_option<"nnf"> option_nnf;
    std::vector<std::string_view> arguments;

    PPsat::cli_parser(option_convert, option_dpll, option_cdcl, option_nnf)
        .parse(argc,
               argv,
               std::back_inserter(arguments),
               PPsat::discard_iterator);

    if (option_convert)
    {
        if (arguments.size() > 2)
        {
            std::cerr << "Invalid argument count: " << arguments.size()
                      << ", quitting.\n";
            return 1;
        }

        std::optional<std::ifstream> input_file;
        if (arguments.size() >= 1)
        {
            input_file.emplace(arguments[0].data());
        }
        std::istream& input = input_file ? *input_file : std::cin;

        std::optional<std::ofstream> output_file;
        if (arguments.size() >= 2)
        {
            output_file.emplace(arguments[1].data());
        }
        std::ostream& output = output_file ? *output_file : std::cout;

        auto status = PPsat::tseitin_translate(input, output, option_nnf);

        if (status == PPsat::error_code::syntax)
        {
            std::cerr << "Parse error encountered, quitting.\n";
            return 2;
        }

        return 0;
    }

    std::cerr << "No option specified.\n";
    return 1;
}
