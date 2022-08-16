#pragma once
#include <PPsat-base/cli/error_handler.hpp>

namespace PPsat_base::cli
{
template <template <typename> typename C1, template <typename> typename C2>
class error_handler_container : public error_handler
{
protected:
    C1<std::string_view> storage_option_argument;
    C2<std::string_view> storage_option;

public:
    void unrecognized_option_argument(
        const std::string_view argument) noexcept override final
    {
        storage_option_argument.push_back(argument);
    }
    void unrecognized_option(
        const std::string_view option) noexcept override final
    {
        storage_option.push_back(option);
    }
};
}
