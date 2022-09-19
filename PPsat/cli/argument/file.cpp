#include "PPsat-base/cli/parameter/simple.hpp"
#include <PPsat/cli/argument/file.hpp>

#include <PPsat-base/logger_subroutine.hpp>

#include <filesystem>

template <typename FStream>
PPsat::cli::argument::file<FStream>::file()
    : PPsat_base::cli::parameter::simple(false)
{}

template <typename FStream>
void PPsat::cli::argument::file<FStream>::parse_path(
    std::filesystem::path path) noexcept
{
    format = path.extension() == ".sat"   ? formula_format::SMTLIB
             : path.extension() == ".cnf" ? formula_format::DIMACS
                                          : formula_format::unspecified;
}

template <typename FStream>
std::pair<PPsat::formula_format,
          typename PPsat_base::cli::argument::file<FStream>::stream_type&>
PPsat::cli::argument::file<FStream>::parsed()
{
    return {format, this->parsed_stream()};
}

template <typename FStream>
typename PPsat_base::cli::argument::file<FStream>::stream_type&
PPsat::cli::argument::file<FStream>::default_stream() const noexcept
{
    if constexpr (std::same_as<FStream, std::ifstream>)
    {
        return std::cin;
    }
    else
    {
        return std::cout;
    }
}

template class PPsat::cli::argument::file<std::ifstream>;
template class PPsat::cli::argument::file<std::ofstream>;
