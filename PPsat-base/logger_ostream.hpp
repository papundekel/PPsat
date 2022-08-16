#pragma once
#include <PPsat-base/logger.hpp>

#include <iosfwd>

namespace PPsat_base
{
class logger_ostream final : public logger
{
    std::ostream& stream;

public:
    logger_ostream(std::ostream& stream) noexcept;

    std::ostream& print_prefix_bare() const override final;
    std::ostream& print_prefix_full() const override final;
};
}
