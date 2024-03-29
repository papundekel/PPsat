#pragma once
#include <PPsat/formula_format.hpp>

#include <PPsat-base/cli/argument/file.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>

namespace PPsat::cli::argument
{
template <typename FStream>
class file
    : public PPsat_base::cli::argument::file<FStream>
    , public PPsat_base::cli::parameter::simple
{
    formula_format format;

public:
    file();

private:
    void parse_path(
        std::filesystem::path argument_path) noexcept override final;

    typename PPsat_base::cli::argument::file<FStream>::stream_type&
    default_stream() const noexcept override final;

public:
    std::pair<formula_format,
              typename PPsat_base::cli::argument::file<FStream>::stream_type&>
    parsed();
};

extern template class PPsat::cli::argument::file<std::ifstream>;
extern template class PPsat::cli::argument::file<std::ofstream>;

using file_in = file<std::ifstream>;
using file_out = file<std::ofstream>;
}
