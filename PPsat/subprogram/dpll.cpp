#include "PPsat/formula.hpp"
#include "PPsat/literal.hpp"
#include "PPsat/literal_pair.hpp"
#include "PPsat/logger.hpp"
#include "PPsat/renaming.hpp"
#include <PPsat/subprogram/dpll.hpp>

#include <PPsat/builder.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener.hpp>
#include <PPsat/factory.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/literal_negated.hpp>
#include <PPsat/logger_subroutine.hpp>
#include <PPsat/renaming_map.hpp>

#include <algorithm>
#include <bits/ranges_algo.h>
#include <bits/ranges_util.h>
#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <type_traits>
#include <vector>

namespace
{
using formula = PPsat::formula;
using literal = PPsat::literal;
using literal_pair = PPsat::literal_pair;
using clause = PPsat::clause;

class heuristic_decision
{
public:
    virtual void init(std::size_t count_variable) = 0;
    virtual void assigned(std::size_t variable) = 0;
    virtual void unassigned(std::size_t variable) = 0;
    virtual std::size_t decision() const = 0;
};

class heuristic_decision_set final : public heuristic_decision
{
    std::set<std::size_t> set;

    void init(std::size_t count_variable) override final
    {
        set.clear();
        for (auto i = 0uz; i != count_variable; ++i)
        {
            set.emplace(i);
        }
    }

    void assigned(std::size_t variable) override final
    {
        set.erase(variable);
    }
    void unassigned(std::size_t variable) override final
    {
        set.emplace(variable);
    }

    std::size_t decision() const override final
    {
        return *set.begin();
    }
};

class literal_adjacency
{
    virtual void init_prepare(std::size_t count_variable) = 0;
    virtual void contained_in_clause(const literal& literal,
                                     const clause& clause) = 0;

public:
    void init(std::size_t count_variable, const formula& formula)
    {
        init_prepare(count_variable);

        formula.for_each(
            [this](const clause& clause)
            {
                clause.for_each(
                    [this, &clause](const literal& literal)
                    {
                        contained_in_clause(literal, clause);
                    });
            });
    }

    virtual void for_each_clause_containing(
        std::size_t variable,
        std::function<void(const clause&, const literal&)> f) const = 0;
};

class literal_adjacency_list final : public literal_adjacency
{
    std::vector<std::vector<
        std::pair<std::reference_wrapper<const clause>, literal_pair>>>
        list;

    void init_prepare(std::size_t count_variable) override final
    {
        list.resize(count_variable);
    }

    void contained_in_clause(const literal& literal,
                             const clause& clause) override final
    {
        list[literal.get_variable()].emplace_back(clause, literal);
    }

public:
    void for_each_clause_containing(
        std::size_t variable,
        std::function<void(const clause&, const literal&)> f)
        const override final
    {
        for (auto& [clause, literal] : list[variable])
        {
            f(clause, literal);
        }
    }
};

enum class clause_category
{
    other,
    unit,
    sat,
    unsat
};

struct decisive_t
{};
constexpr inline decisive_t decisive;

bool operator==(clause_category category, decisive_t) noexcept
{
    return category == clause_category::sat ||
           category == clause_category::unsat;
}

class assignment
{
    virtual std::pair<clause_category, literal_pair>
    get_category_and_first_literal_impl() const noexcept = 0;

public:
    virtual bool is_sat() const noexcept = 0;

    std::pair<clause_category, literal_pair> get_category_and_first_literal()
        const noexcept
    {
        return is_sat() ? std::make_pair(clause_category::sat, literal_pair{})
                        : get_category_and_first_literal_impl();
    }

    virtual void assign(const literal& literal_assigned,
                        const literal& literal_in_clause) = 0;
    virtual void unassign(const literal& literal_unassigned,
                          const literal& literal_in_clause) = 0;
};

class assignment_sets final : public assignment
{
    using set = std::set<literal_pair>;

    set set_unassigned;
    set set_assigned_false;
    set set_assigned_true;

public:
    assignment_sets(const clause& clause)
        : set_unassigned()
        , set_assigned_false()
        , set_assigned_true()
    {
        clause.for_each(
            [this](const literal& literal)
            {
                set_unassigned.emplace(literal);
            });
    }

    bool is_sat() const noexcept override final
    {
        return !set_assigned_true.empty();
    }

    std::pair<clause_category, literal_pair>
    get_category_and_first_literal_impl() const noexcept override final
    {
        const auto unassigned_count = set_unassigned.size();
        if (unassigned_count == 0)
        {
            return {clause_category::unsat, {}};
        }
        else
        {
            return {unassigned_count == 1 ? clause_category::unit
                                          : clause_category::other,
                    *set_unassigned.begin()};
        }
    }

    void assign(const literal& literal_assigned,
                const literal& literal_in_clause) override final
    {
        set_unassigned.erase(literal_in_clause);
        if (literal_assigned.is_positive() == literal_in_clause.is_positive())
        {
            set_assigned_true.emplace(literal_in_clause);
        }
        else
        {
            set_assigned_false.emplace(literal_in_clause);
        }
    }

    void unassign(const literal& literal_unassigned,
                  const literal& literal_in_clause) override final
    {
        if (literal_unassigned.is_positive() == literal_in_clause.is_positive())
        {
            set_assigned_true.erase(literal_in_clause);
        }
        else
        {
            set_assigned_false.erase(literal_in_clause);
        }
        set_unassigned.emplace(literal_in_clause);
    }
};

class assignment_map
{
    virtual std::optional<std::reference_wrapper<assignment>> contains_clause(
        std::function<bool(const assignment&)> p) const = 0;

public:
    virtual void init(const PPsat::formula& formula) = 0;

    bool contains_unsat_clause() const
    {
        return contains_clause(
                   [](const assignment& assignment)
                   {
                       return assignment.get_category_and_first_literal()
                                  .first == clause_category::unsat;
                   })
            .has_value();
    }

    std::optional<literal_pair> find_unit_clause() const
    {
        const auto assignment_opt = contains_clause(
            [](const assignment& assignment)
            {
                return assignment.get_category_and_first_literal().first ==
                       clause_category::unit;
            });

        if (!assignment_opt)
        {
            return {};
        }

        return assignment_opt->get().get_category_and_first_literal().second;
    }

    virtual assignment& get_assignment(const clause& clause) = 0;
};

class assignment_map_map final : public assignment_map
{
    std::map<const PPsat::clause*, assignment_sets> map;

    void init(const PPsat::formula& formula) override final
    {
        formula.for_each(
            [this](const clause& clause)
            {
                map.try_emplace(&clause, clause);
            });
    }

    std::optional<std::reference_wrapper<assignment>> contains_clause(
        std::function<bool(const assignment&)> p) const override final
    {
        auto i =
            std::ranges::find_if(map, p, &decltype(map)::value_type::second);

        if (i == map.end())
        {
            return {};
        }

        return (assignment&)i->second;
    }

    assignment& get_assignment(const clause& clause) override final
    {
        return map.at(&clause);
    }
};

class dpll_structure
{
    literal_adjacency& adjacency;
    assignment_map& assignments;
    heuristic_decision& heuristic;
    std::vector<literal_pair> stack_assignment;
    std::vector<std::size_t> stack_decision;
    std::size_t count_clause;
    std::size_t count_clause_sat;
    const PPsat::logger& logger;

public:
    dpll_structure(std::size_t variable_count,
                   const formula& formula,
                   assignment_map& assignments,
                   literal_adjacency& adjacency,
                   class heuristic_decision& heuristic,
                   const PPsat::logger& logger)
        : assignments(assignments)
        , adjacency(adjacency)
        , heuristic(heuristic)
        , stack_assignment()
        , stack_decision()
        , count_clause(formula.clause_count())
        , count_clause_sat(0)
        , logger(logger)
    {
        assignments.init(formula);

        if (assignments.contains_unsat_clause())
        {
            return;
        }

        adjacency.init(variable_count, formula);
        heuristic.init(variable_count);
    }

    std::pair<clause_category, literal_pair> assign(
        const literal& literal_assigned)
    {
        bool made_unsat = false;
        bool made_unit = false;
        bool made_unknown = false;
        literal_pair literal_first;

        adjacency.for_each_clause_containing(
            literal_assigned.get_variable(),
            [this,
             &literal_assigned,
             &made_unsat,
             &made_unit,
             &made_unknown,
             &literal_first](const clause& clause,
                             const literal& literal_in_clause)
            {
                auto& assignment = assignments.get_assignment(clause);

                const bool was_sat = assignment.is_sat();

                assignment.assign(literal_assigned, literal_in_clause);

                const auto [category, literal] =
                    assignment.get_category_and_first_literal();

                if (!was_sat && category == clause_category::sat)
                {
                    ++count_clause_sat;
                }

                if (category == clause_category::unsat)
                {
                    made_unsat = true;
                }
                else if (category != clause_category::sat)
                {
                    made_unknown = true;

                    if (category == clause_category::unit)
                    {
                        literal_first = literal;
                        made_unit = true;
                    }
                }
            });

        heuristic.assigned(literal_assigned.get_variable());
        stack_assignment.push_back(literal_assigned);

        return {made_unsat     ? clause_category::unsat
                : made_unit    ? clause_category::unit
                : made_unknown ? clause_category::other
                               : clause_category::sat,
                literal_first};
    }

    literal_pair unassign()
    {
        const literal& literal_unassigned = stack_assignment.back();
        stack_assignment.pop_back();

        logger << "unassigned " << literal_unassigned << "\n";

        heuristic.unassigned(literal_unassigned.get_variable());

        adjacency.for_each_clause_containing(
            literal_unassigned.get_variable(),
            [this, &literal_unassigned](const clause& clause,
                                        const literal& literal_in_clause)
            {
                auto& assignment = assignments.get_assignment(clause);

                const bool was_sat = assignment.is_sat();

                assignment.unassign(literal_unassigned, literal_in_clause);

                if (was_sat && !assignment.is_sat())
                {
                    --count_clause_sat;
                }
            });

        return literal_unassigned;
    }

    clause_category unit_propagate(const literal& literal_starting)
    {
        literal_pair literal_current = literal_starting;

        while (true)
        {
            auto [category, literal_future] = assign(literal_current);

            logger << "unit " << literal_current << "\n";

            if (category == decisive)
            {
                return category;
            }
            else if (category != clause_category::unit)
            {
                break;
            }

            literal_current = literal_future;
        }

        return clause_category::other;
    }

    clause_category decide(const literal& literal)
    {
        stack_decision.push_back(stack_assignment.size());

        return assign(literal).first;
    }

    std::optional<std::size_t> backtrack()
    {
        std::size_t decided_variable = 0;
        bool was_decision_true = false;

        do
        {
            if (stack_decision.empty())
            {
                return {};
            }

            const auto count_before_decision = stack_decision.back();
            stack_decision.pop_back();

            literal_pair decided_literal;

            while (stack_assignment.size() != count_before_decision)
            {
                decided_literal = unassign();
            }

            decided_variable = decided_literal.get_variable();
            was_decision_true = decided_literal.is_positive();

        } while (was_decision_true);

        logger << "backtracked\n";

        return decided_variable;
    }

    bool solve()
    {
        bool backtracked = false;
        std::size_t decision_variable;

        while (true)
        {
            clause_category category = clause_category::other;

            if (!backtracked)
            {
                const auto literal_unit_opt = assignments.find_unit_clause();

                if (literal_unit_opt)
                {
                    category = unit_propagate(*literal_unit_opt);
                }
            }

            if (category != decisive)
            {
                if (!backtracked)
                {
                    decision_variable = heuristic.decision();
                }

                category = decide(literal_pair{decision_variable, backtracked});

                logger << "decided "
                       << literal_pair{decision_variable, backtracked} << "\n";
            }

            if (count_clause == count_clause_sat)
            {
                return true;
            }
            else if (category == clause_category::unsat)
            {
                for (auto& x : stack_assignment)
                {
                    logger << "A " << x << "\n";
                }

                auto decision_variable_new_opt = backtrack();
                backtracked = true;

                if (!decision_variable_new_opt)
                {
                    return false;
                }

                decision_variable = *decision_variable_new_opt;
            }
            else
            {
                backtracked = false;
            }
        }
    }

    void for_each_assignment(std::function<void(const literal&)> f) const
    {
        std::ranges::for_each(stack_assignment, f);
    }
};
}

PPsat::subcommand_result PPsat::subprogram::dpll_unparsed(
    const logger& logger_outer,
    cli::arguments& arguments,
    options& options)
{
    if (!options.dpll)
    {
        return {};
    }

    const auto& logger_inner = logger_subroutine(logger_outer, "dpll");

    PPsat::cli::argument::file_in argument_file_in;

    const auto success =
        arguments.parse(logger_inner, std::array{std::ref(argument_file_in)});

    if (!success)
    {
        logger_inner << "Skipping the subprogram.\n";

        return 1;
    }

    const auto builder =
        create_builder(options.format, argument_file_in, options.nnf);

    auto& formula_input = argument_file_in.parsed_stream();

    PPsat::formula_simple formula;
    const auto renaming = builder->create_ref();

    const auto result =
        builder->read(logger_inner, formula_input, formula, renaming);

    if (!result)
    {
        logger_inner << "Skipping DPLL.\n";

        return 2;
    }

    const auto variable_count = result.get_variable_count();

    assignment_map_map assignments;
    literal_adjacency_list adjacency;
    heuristic_decision_set heuristic;
    dpll_structure structure(variable_count,
                             formula,
                             assignments,
                             adjacency,
                             heuristic,
                             logger_inner);

    const auto satisfiable = structure.solve();

    formula.write_DIMACS(std::cerr,
                         (std::ostream & (&)(std::ostream&, const literal&))
                             PPsat::operator<<);

    if (satisfiable)
    {
        std::cout << "s SATISFIABLE\n";
        std::cout << "v ";

        std::vector<renamed_literal> model;

        structure.for_each_assignment(
            [&renaming, &model](const literal& literal)
            {
                auto lit_opt = renaming->get_literal(literal);
                if (lit_opt)
                {
                    model.push_back(*lit_opt);
                }
            });

        std::sort(model.begin(), model.end());

        for (const auto& lit : model)
        {
            std::cout << lit << " ";
        }

        std::cout << "0\n";
    }
    else
    {
        std::cout << "s UNSATISFIABLE\n";
    }

    return 0;
}
