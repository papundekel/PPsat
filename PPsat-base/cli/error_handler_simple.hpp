#pragma once
#include <PPsat-base/cli/error_handler_container.hpp>
#include <PPsat-base/containers.hpp>

#include <iosfwd>

namespace PPsat_base::cli
{
class error_handler_simple final
    : public error_handler_container<vector, vector>
{
    std::ostream& out;

public:
    error_handler_simple(std::ostream& out) noexcept;

    void handle() const noexcept override final;
};
}
