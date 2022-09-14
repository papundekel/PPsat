#include <PPsat/cli/options.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/output_type.hpp>
#include <PPsat/subprogram.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/view_any.hpp>

#include <array>
#include <tuple>
#include <utility>

PPsat::cli::options::options()
    : options_base{
          {"help", false},
          {"subprogram",
           subprogram::selection::solve,
           std::array{std::make_pair("convert", subprogram::selection::convert),
                      std::make_pair("solve", subprogram::selection::solve)}},
          {"virtual", false},
          {"nnf", false},
          {"assume", false},
          {"cdcl", false},
          {"restart", 2.5},
          {"random", 0},
          {"decision",
           decision::type::JW_static,
           std::array{
               std::make_pair("trivial", decision::type::trivial),
               std::make_pair("deterministic", decision::type::deterministic),
               std::make_pair("random", decision::type::random),
               std::make_pair("JW_static", decision::type::JW_static),
               std::make_pair("VSIDS", decision::type::VSIDS)}},
          {"format",
           formula_format::DIMACS,
           std::array{std::make_pair("dimacs", formula_format::DIMACS),
                      std::make_pair("smtlib", formula_format::SMTLIB)}},
          {"clause",
           PPsat_base::clause::type::watched_literals,
           std::array{
               std::make_pair("basic", PPsat_base::clause::type::basic),
               std::make_pair("counting", PPsat_base::clause::type::counting),
               std::make_pair("watched_literals",
                              PPsat_base::clause::type::watched_literals)}},
          {"adjacency",
           adjacency::type::set_unordered,
           std::array{std::make_pair("list", adjacency::type::list),
                      std::make_pair("set", adjacency::type::set),
                      std::make_pair("set_unordered",
                                     adjacency::type::set_unordered)}},
          {"output",
           output_type::human_readable,
           std::array{
               std::make_pair("human_readable", output_type::human_readable),
               std::make_pair("csv", output_type::csv)}}}
{}
