#include <PPsat-base/logger_subroutine.hpp>

#include <ostream>

PPsat_base::logger_subroutine::logger_subroutine(const logger& logger_outer,
                                                 std::string_view name)
    : logger_outer(logger_outer)
    , name(name)
{}

std::ostream& PPsat_base::logger_subroutine::print_prefix_bare() const
{
    return logger_outer.print_prefix_full() << name;
}

std::ostream& PPsat_base::logger_subroutine::print_prefix_full() const
{
    return print_prefix_bare() << "::";
}
