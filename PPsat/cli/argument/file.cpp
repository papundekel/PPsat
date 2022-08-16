#include "PPsat-base/logger_subroutine.hpp"
#include <PPsat/cli/argument/file.hpp>
#include <filesystem>

template <typename FStream>
void PPsat::cli::argument::file<FStream>::parse_path(
    std::filesystem::path path) noexcept
{
    format = path.extension() == ".sat"   ? formula_format::SMTLIB
             : path.extension() == ".cnf" ? formula_format::DIMACS
                                          : formula_format::DIMACS;
}

template <typename FStream>
PPsat::formula_format PPsat::cli::argument::file<FStream>::parsed_format() const
{
    return format;
}

template class PPsat::cli::argument::file<std::ifstream>;
template class PPsat::cli::argument::file<std::ofstream>;
