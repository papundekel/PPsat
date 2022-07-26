#pragma once
#include <string_view>

namespace PPsat::cli
{
    class error_handler
    {
    public:
        virtual void unrecognized_argument(std::string_view argument) noexcept = 0;
        virtual void unrecognized_option_argument(std::string_view argument) noexcept = 0;
        virtual void handle() const noexcept = 0;
    };
}
