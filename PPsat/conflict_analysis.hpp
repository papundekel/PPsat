#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/optional.hpp>

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

    virtual void reset() = 0;

    virtual PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const clause& antecedent) = 0;

    virtual std::pair<PPsat_base::optional<const clause&>, std::list<unit>>
    post_backtrack(solver& solver, literal literal) = 0;

    virtual std::list<unit> find_unary_unit() const = 0;

    virtual void restart() = 0;

    virtual ~conflict_analysis() = default;
};
}
