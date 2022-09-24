#pragma once
#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/variable.hpp>

#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/policies.hpp>

#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class formula;

class decision_VSIDS final : public decision
{
    class comparer
    {
    public:
        bool operator()(const variable* a, const variable* b) const;
    };

    formula& formula_;
    boost::heap::binomial_heap<variable*, boost::heap::compare<comparer>> set;
    std::unordered_map<variable*, decltype(set)::handle_type> handles;
    double bump;
    double decay;

    void emplace_variable(variable& variable);

public:
    decision_VSIDS(formula& formula, const cli::parameters_value& parameters);

    void reset(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
