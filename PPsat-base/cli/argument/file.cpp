#include <PPsat/cli/argument/file.hpp>

#include <PPsat-base/logger_subroutine.hpp>

template <typename FStream>
bool PPsat_base::cli::argument::file<FStream>::parse(
    const PPsat_base::logger& logger_outer,
    std::string_view argument_path) noexcept
{
    const auto path = std::filesystem::path(argument_path);

    parse_path(path);

    stream_file = {path};

    const auto success = !!stream_file;

    if (!success)
    {
        PPsat_base::logger_subroutine(logger_outer, "file")
            << "Error handling file \"" << argument_path << "\".\n";
    }

    return success;
}

template <typename FStream>
typename PPsat_base::cli::argument::file<FStream>::stream_type&
PPsat_base::cli::argument::file<FStream>::parsed_stream()
{
    return stream_file;
}

template class PPsat_base::cli::argument::file<std::ifstream>;
template class PPsat_base::cli::argument::file<std::ofstream>;
