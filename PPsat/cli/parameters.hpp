#pragma once
#include <PPsat/adjacency.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/output_format.hpp>
#include <PPsat/subprogram.hpp>

#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/bool.hpp>
#include <PPsat-base/cli/option/enum.hpp>
#include <PPsat-base/cli/option/named.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_double.hpp>
#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>
#include <PPsat-base/cli/option/simple_named_int.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/constant_string.hpp>
#include <PPsat-base/tuple_named.hpp>

#include <array>
#include <functional>
#include <tuple>
#include <utility>

namespace PPsat::cli
{
class parameters_value
{
public:
    std::pair<formula_format, std::istream&> input;
    std::pair<formula_format, std::ostream&> output;
    bool input_present;

    std::size_t iterations;
    std::size_t random_seed;
    double restart;
    bool help;
    bool nnf;
    bool assume;
    output_format format_output;

    subprogram::selection subprogram;
    decision::type decision_;
    formula_format format_input;
    clause::type clause_;
    adjacency::type adjancency;
    bool virtual_;
    bool cdcl;

    auto as_comparable() const noexcept
    {
        return std::forward_as_tuple(subprogram,
                                     decision_,
                                     format_input,
                                     clause_,
                                     adjancency,
                                     virtual_,
                                     cdcl);
    }

    bool operator<(const parameters_value& other) const noexcept
    {
        return as_comparable() < other.as_comparable();
    }
};

using parameters_base_options = std::tuple<
    PPsat_base::cli::option::simple_named_int,
    PPsat_base::cli::option::simple_named_int,
    PPsat_base::cli::option::simple_named_double,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_enum_typed<output_format>,
    PPsat_base::cli::option::simple_named_enum_typed<subprogram::selection>,
    PPsat_base::cli::option::simple_named_enum_typed<decision::type>,
    PPsat_base::cli::option::simple_named_enum_typed<formula_format>,
    PPsat_base::cli::option::simple_named_enum_typed<clause::type>,
    PPsat_base::cli::option::simple_named_enum_typed<adjacency::type>,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_bool>;

using parameters_base_arguments =
    std::tuple<PPsat::cli::argument::file_in, PPsat::cli::argument::file_out>;

class parameters
    : public parameters_base_options
    , public parameters_base_arguments
    , public PPsat_base::cli::parameters<parameters_value>
{
    template <typename Base>
    void for_each(auto f)
    {
        return std::apply(
            [&f](auto&... option)
            {
                (f(option), ...);
            },
            static_cast<Base&>(*this));
    }

public:
    parameters();

    parameters_value parsed() override final
    {
        auto parameters = std::apply(
            [this](const auto&... option)
            {
                return std::apply(
                    [&option...](PPsat::cli::argument::file_in& file_in,
                                 PPsat::cli::argument::file_out& file_out)
                    {
                        return parameters_value{file_in.parsed(),
                                                file_out.parsed(),
                                                file_in.is_present(),
                                                option.parsed()...};
                    },
                    static_cast<parameters_base_arguments&>(*this));
            },
            static_cast<parameters_base_options&>(*this));

        if (parameters.format_input == formula_format::unspecified)
        {
            if (parameters.input_present)
            {
                parameters.format_input = parameters.input.first;
            }
            else
            {
                switch (parameters.subprogram)
                {
                    case subprogram::selection::convert:
                        parameters.format_input = formula_format::SMTLIB;
                        break;
                    case subprogram::selection::solve:
                        parameters.format_input = formula_format::DIMACS;
                        break;
                }
            }
        }

        return parameters;
    }

    void options(std::function<void(PPsat_base::cli::option_&)> f) noexcept
        override final
    {
        for_each<parameters_base_options>(std::move(f));
    }

    void arguments(std::function<void(PPsat_base::cli::argument_&)> f) noexcept
        override final
    {
        for_each<parameters_base_arguments>(std::move(f));
    }
};
}
