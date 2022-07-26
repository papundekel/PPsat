#pragma once
#include <PPsat/cli/error_handler_container.hpp>

#include <vector>
#include <iostream>

namespace PPsat
{
    class error_handler : public cli::error_handler_container<std::vector, std::vector>
    {
        std::ostream& out;

    public:
        error_handler(std::ostream& out) noexcept;

        void handle() const noexcept override final;
    };
}