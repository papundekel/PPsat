#include <PPsat/cli/argument/file.hpp>

template <typename Stream>
bool PPsat::cli::argument::file<Stream>::parse(
    std::string_view argument_path) noexcept
{
    const auto path = std::filesystem::path(argument_path);

    format = path.extension() == ".sat" ? formula_format::SMTLIB
                                        : formula_format::DIMACS;

    stream = Stream(path);

    return !!stream;
}

template <typename Stream>
PPsat::formula_format& PPsat::cli::argument::file<Stream>::parsed_format()
{
    return format;
}

template class PPsat::cli::argument::file<std::ifstream>;
template class PPsat::cli::argument::file<std::ofstream>;

std::istream& PPsat::cli::argument::file_in::parsed_stream()
{
    return is_present() ? stream : std::cin;
}

std::ostream& PPsat::cli::argument::file_out::parsed_stream()
{
    return is_present() ? stream : std::cout;
}
