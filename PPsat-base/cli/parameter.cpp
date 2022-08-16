#include <PPsat-base/cli/parameter.hpp>

PPsat_base::cli::parameter_::operator bool() const noexcept
{
    return is_present();
}
