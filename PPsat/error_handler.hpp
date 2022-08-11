#pragma once
#include <PPsat/cli/error_handler_container.hpp>
#include <PPsat/vector.hpp>

#include <iostream>
#include <vector>

namespace PPsat
{
class error_handler final
    : public cli::error_handler_container<PPsat::vector, PPsat::vector>
{
    std::ostream& out;

public:
    error_handler(std::ostream& out) noexcept;

    void handle() const noexcept override final;
};
}
