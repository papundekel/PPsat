#include "PPsat/logger_subroutine.hpp"
#include <PPsat/cli/argument/file.hpp>

template <typename Stream>
bool PPsat::cli::argument::file<Stream>::parse(
    const logger& logger_outer,
    std::string_view argument_path) noexcept
{
    const auto path = std::filesystem::path(argument_path);

    format = path.extension() == ".sat"   ? formula_format::SMTLIB
             : path.extension() == ".cnf" ? formula_format::DIMACS
                                          : formula_format::DIMACS;

    stream = Stream(path);

    const auto success = !!stream;

    if (!success)
    {
        logger_subroutine(logger_outer, "file")
            << "Error handling file \"" << argument_path << "\".\n";
    }

    return success;
}

template <typename Stream>
PPsat::formula_format PPsat::cli::argument::file<Stream>::parsed_format() const
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
