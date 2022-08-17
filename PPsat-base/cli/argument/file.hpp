#pragma once
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <string_view>
#include <type_traits>

namespace PPsat_base::cli::argument
{
template <typename FStream>
class file : public virtual argument_
{
    using stream_type = std::conditional_t<std::same_as<FStream, std::ifstream>,
                                           std::istream,
                                           std::ostream>;

protected:
    FStream stream_file;

private:
    virtual void parse_path(std::filesystem::path path) noexcept = 0;

public:
    bool parse(const PPsat_base::logger& logger,
               std::string_view argument_path) noexcept override final;

    stream_type& parsed_stream();
};
}

extern template class PPsat_base::cli::argument::file<std::ifstream>;
extern template class PPsat_base::cli::argument::file<std::ofstream>;
