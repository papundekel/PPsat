#pragma once
#include <PPsat/decision.hpp>

#include "PPsat-base/formula.hpp"
#include "PPsat-base/variable.hpp"

#include <boost/heap/binomial_heap.hpp>

#include <boost/heap/policies.hpp>
#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class decision_VSIDS final : public decision
{
    class comparer
    {
    public:
        bool operator()(const PPsat_base::variable* a,
                        const PPsat_base::variable* b) const;
    };

    PPsat_base::formula& formula;
    boost::heap::binomial_heap<PPsat_base::variable*,
                               boost::heap::compare<comparer>>
        set;
    std::unordered_map<PPsat_base::variable*, decltype(set)::handle_type>
        handles;
    double bump;
    double decay;

public:
    decision_VSIDS(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
