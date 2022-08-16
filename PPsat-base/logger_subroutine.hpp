#pragma once
#include <PPsat-base/logger.hpp>

namespace PPsat_base
{
class logger_subroutine final : public logger
{
    const logger& logger_outer;
    std::string_view name;

public:
    logger_subroutine(const logger& logger_outer, std::string_view name);

    std::ostream& print_prefix_bare() const override final;
    std::ostream& print_prefix_full() const override final;
};
}
