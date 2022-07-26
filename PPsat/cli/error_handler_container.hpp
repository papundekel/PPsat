#pragma once
#include <PPsat/cli/error_handler.hpp>

namespace PPsat::cli
{
    template <template <typename> typename C1, template <typename> typename C2>
    class error_handler_container : public error_handler
    {
    protected:
        C1<std::string_view> storage_argument;
        C2<std::string_view> storage_option_argument;

    public:
        void unrecognized_argument(std::string_view argument) noexcept override final
        {
            storage_argument.push_back(argument);
        }
        void unrecognized_option_argument(std::string_view argument) noexcept override final
        {
            storage_option_argument.push_back(argument);
        }
    };
}
