#pragma once
#include <PPsat/logger.hpp>

#include <iosfwd>

namespace PPsat
{
class logger_ostream : public logger
{
    std::ostream& stream;

public:
    logger_ostream(std::ostream& stream) noexcept;

    std::ostream& print_prefix_bare() const override final;
    std::ostream& print_prefix_full() const override final;
};
}
