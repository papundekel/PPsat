#pragma once
#include "PPsat-base/clause.hpp"
#include "PPsat-base/optional.hpp"
#include "PPsat-base/unit.hpp"
#include <PPsat-base/literal.hpp>

#include <optional>
#include <utility>

namespace PPsat
{
class solver;

class conflict_analysis
{
public:
    enum class type
    {
        dpll,
        uip,
    };

    virtual PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const PPsat_base::clause& antecedent) = 0;

    virtual std::pair<PPsat_base::optional<const PPsat_base::clause&>,
                      std::list<PPsat_base::unit>>
    post_backtrack(solver& solver, PPsat_base::literal literal) = 0;

    virtual std::list<PPsat_base::unit> find_unary_unit() const = 0;

    virtual void restart() = 0;

    virtual ~conflict_analysis() = default;
};
}
