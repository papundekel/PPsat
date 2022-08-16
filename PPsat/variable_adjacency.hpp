#pragma once
#include <PPsat-base/variable.hpp>

#include <functional>
#include <utility>
#include <vector>

namespace PPsat
{
class variable_adjacency : public virtual PPsat_base::variable
{
    assignment assignment_current;
    std::vector<std::pair<std::reference_wrapper<PPsat_base::clause>, bool>>
        adjacency;

public:
    variable_adjacency() noexcept;

private:
    void set_assignment(assignment assignment) override final;
    assignment get_assignment() const override final;

    void for_each_clause_containing(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void register_containing_clause(PPsat_base::clause& clause,
                                    bool positive) override final;
};
}
