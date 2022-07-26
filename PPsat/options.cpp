#include <PPsat/options.hpp>

std::array<std::reference_wrapper<PPsat::cli::option_>, 7>
PPsat::options::as_range() noexcept
{
    return {help, convert, dpll, cdcl, nnf, watched_literals, format};
}
