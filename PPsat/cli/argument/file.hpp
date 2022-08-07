#pragma once
#include <PPsat/cli/argument.hpp>
#include <PPsat/cli/parameter/simple.hpp>
#include <PPsat/formula_format.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>

namespace PPsat::cli::argument
{
template <typename Stream>
class file : public parameter::simple<argument_>
{
protected:
    Stream stream;
    formula_format format;

public:
    bool parse(std::string_view argument_path) noexcept override final;

    formula_format& parsed_format();
};

extern template class file<std::ifstream>;
extern template class file<std::ofstream>;

class file_in : public file<std::ifstream>
{
public:
    std::istream& parsed_stream();
};

class file_out : public file<std::ofstream>
{
public:
    std::ostream& parsed_stream();
};
}
