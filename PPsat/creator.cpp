#include <PPsat/adjacency.hpp>
#include <PPsat/assumptions.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/clause_picker.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/conflict_analysis_dpll.hpp>
#include <PPsat/conflict_analysis_picker.hpp>
#include <PPsat/conflict_analysis_uip.hpp>
#include <PPsat/creator.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/decision_JW_static.hpp>
#include <PPsat/decision_VSIDS.hpp>
#include <PPsat/decision_assume.hpp>
#include <PPsat/decision_deterministic.hpp>
#include <PPsat/decision_picker.hpp>
#include <PPsat/decision_priority.hpp>
#include <PPsat/decision_random.hpp>
#include <PPsat/decision_trivial.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/formula_picker.hpp>
#include <PPsat/output_format.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/restart_strategy_geometric.hpp>
#include <PPsat/restart_strategy_never.hpp>
#include <PPsat/restart_strategy_picker.hpp>
#include <PPsat/solver_impl.hpp>
#include <PPsat/solver_picker.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable_adjacency_picker.hpp>

#include <PPsat-base/tuple_cart.hpp>
#include <PPsat-base/value_t.hpp>

#include <memory>
#include <tuple>

namespace
{
template <PPsat::cli::parameters_value parameters>
class creator_impl : public PPsat::creator
{
    using clauses_t = PPsat::clauses_picker<parameters>;
    using variables_t = PPsat::variables_picker<parameters>;
    using formula_t = PPsat::formula_picker<parameters>;
    using decision_t = PPsat::decision_picker<parameters>;
    using conflict_analysis_t = PPsat::conflict_analysis_picker<parameters>;
    using restart_strategy_t = PPsat::restart_strategy_picker<parameters>;

public:
    PPsat_base::unique_ref<PPsat_base::factory<PPsat::formula::factory_clause>>
    clauses_factory() const override final
    {
        return std::make_unique<PPsat_base::factory<
            PPsat::formula::factory_clause>::impl<clauses_t>>();
    }

    PPsat_base::unique_ref<PPsat::formula::factory_variable> variables()
        const override final
    {
        return std::make_unique<variables_t>();
    }

    PPsat_base::unique_ref<PPsat::formula> formula(
        const PPsat::preprocessor& preprocessor,
        PPsat::formula::factory_clause& clauses,
        PPsat::formula::factory_variable& variables) const override final
    {
        return std::make_unique<formula_t>(
            preprocessor,
            static_cast<clauses_t&>(clauses),
            static_cast<variables_t&>(variables));
    }

    PPsat_base::unique_ref<PPsat::decision> decision(
        PPsat::formula& formula,
        PPsat::assumptions* assumption,
        const PPsat::cli::parameters_value& parameters_) const override final
    {
        auto main =
            std::make_unique<PPsat::decision_picker<parameters>>(formula,
                                                                 parameters_);

        if (!parameters_.assume)
        {
            return main;
        }
        else
        {
            return std::make_unique<PPsat::decision_priority>(
                std::make_unique<PPsat::decision_assume>(formula, *assumption),
                std::move(main));
        }
    }

    PPsat_base::unique_ref<PPsat::conflict_analysis> analysis(
        const PPsat_base::factory<PPsat::formula::factory_clause>&
            clauses_factory_factory,
        PPsat::decision& decision) const override final
    {
        return std::make_unique<PPsat::conflict_analysis_picker<parameters>>(
            clauses_factory_factory,
            decision);
    }

    PPsat_base::unique_ref<PPsat::restart_strategy> restarts(
        const PPsat::cli::parameters_value& parameters_) const override final
    {
        return std::make_unique<PPsat::restart_strategy_picker<parameters>>(
            parameters_);
    }

    PPsat_base::unique_ref<PPsat::solver> solver(
        PPsat::formula& formula,
        PPsat::decision& decision,
        PPsat::conflict_analysis& analysis,
        PPsat::restart_strategy& restarts) const override final
    {
        return std::make_unique<PPsat::solver_picker<parameters>>(
            static_cast<formula_t&>(formula),
            static_cast<decision_t&>(decision),
            static_cast<conflict_analysis_t&>(analysis),
            static_cast<restart_strategy_t&>(restarts));
    }
};
}

constexpr inline auto bool_ =
    std::make_tuple(PPsat_base::value_v<false>, PPsat_base::value_v<true>);

constexpr inline auto combos1 = PPsat_base::tuple_cart(
    std::make_tuple(PPsat_base::value_v<PPsat::subprogram::selection::convert>,
                    PPsat_base::value_v<PPsat::subprogram::selection::solve>),
    std::make_tuple(PPsat_base::value_v<PPsat::decision::type::deterministic>,
                    PPsat_base::value_v<PPsat::decision::type::trivial>,
                    PPsat_base::value_v<PPsat::decision::type::random>,
                    PPsat_base::value_v<PPsat::decision::type::JW_static>,
                    PPsat_base::value_v<PPsat::decision::type::VSIDS>),
    std::make_tuple(PPsat_base::value_v<PPsat::formula_format::DIMACS>,
                    PPsat_base::value_v<PPsat::formula_format::SMTLIB>));

constexpr inline auto combos2 = PPsat_base::tuple_cart(
    std::make_tuple(PPsat_base::value_v<PPsat::clause::type::basic>,
                    PPsat_base::value_v<PPsat::clause::type::counting>,
                    PPsat_base::value_v<PPsat::clause::type::watched_literals>),
    std::make_tuple(
        PPsat_base::value_v<PPsat::adjacency::type::vector>,
        PPsat_base::value_v<PPsat::adjacency::type::list>,
        PPsat_base::value_v<PPsat::adjacency::type::set>,
        PPsat_base::value_v<PPsat::adjacency::type::set_unordered>));

constexpr inline auto combos3 = PPsat_base::tuple_cart(bool_, bool_);

static std::map<PPsat::cli::parameters_value, std::unique_ptr<PPsat::creator>>
    map = []()
{
    std::map<PPsat::cli::parameters_value, std::unique_ptr<PPsat::creator>> map;

    std::apply(
        [&map](auto... tuples1)
        {
            (std::apply(
                 [&map](auto... values1)
                 {
                     std::apply(
                         [&map](auto... tuples2)
                         {
                             (std::apply(
                                  [&map](auto... values2)
                                  {
                                      std::apply(
                                          [&map](auto... tuples3)
                                          {
                                              (std::apply(
                                                   [&map](auto... values3)
                                                   {
                                                       constexpr PPsat::cli::
                                                           parameters_value val{
                                                               {PPsat::
                                                                    formula_format::
                                                                        DIMACS,
                                                                std::cin},
                                                               {PPsat::
                                                                    formula_format::
                                                                        DIMACS,
                                                                std::cout},
                                                               false,
                                                               0,
                                                               0,
                                                               0,
                                                               false,
                                                               false,
                                                               false,
                                                               PPsat::output_format::
                                                                   human_readable,
                                                               decltype(values1)::
                                                                   value...,
                                                               decltype(values2)::
                                                                   value...,
                                                               decltype(values3)::
                                                                   value...};

                                                       map.try_emplace(
                                                           val,
                                                           std::make_unique<
                                                               creator_impl<
                                                                   val>>());
                                                   },
                                                   tuples3),
                                               ...);
                                          },
                                          combos3);
                                  },
                                  tuples2),
                              ...);
                         },
                         combos2);
                 },
                 tuples1),
             ...);
        },
        combos1);

    return map;
}();

PPsat::creator& PPsat::creator::pick(const cli::parameters_value& parameters)
{
    return *map.at(parameters);
}
