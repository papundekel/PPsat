#include "PPsat/decision_type.hpp"
#include "PPsat/output_type.hpp"
#include "PPsat/subprogram.hpp"
#include <PPsat/cli/options.hpp>

#include <PPsat/clause_type.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/view_any.hpp>
#include <array>
#include <tuple>
#include <utility>

PPsat::cli::options::options()
    : options_base{
          {"help"},
          {"subprogram",
           subprogram::selection::solve,
           std::array{std::make_pair("convert", subprogram::selection::convert),
                      std::make_pair("solve", subprogram::selection::solve)}},
          {"nnf"},
          {"assume"},
          {"cdcl"},
          {"restart", 2.5},
          {"random", 0},
          {"decision",
           decision_type::VSIDS,
           std::array{std::make_pair("trivial", decision_type::trivial),
                      std::make_pair("random", decision_type::random),
                      std::make_pair("JW_static", decision_type::JW_static),
                      std::make_pair("VSIDS", decision_type::VSIDS)}},
          {"format",
           formula_format::DIMACS,
           std::array{std::make_pair("dimacs", formula_format::DIMACS),
                      std::make_pair("smtlib", formula_format::SMTLIB)}},
          {"clause",
           clause_type::watched_literals,
           std::array{std::make_pair("basic", clause_type::basic),
                      std::make_pair("counting", clause_type::counting),
                      std::make_pair("watched_literals",
                                     clause_type::watched_literals)}},
          {"adjacency",
           adjacency_type::set_unordered,
           std::array{
               std::make_pair("list", adjacency_type::list),
               std::make_pair("set", adjacency_type::set),
               std::make_pair("set_unordered", adjacency_type::set_unordered)}},
          {"output",
           output_type::human_readable,
           std::array{
               std::make_pair("human_readable", output_type::human_readable),
               std::make_pair("csv", output_type::csv)}}}
{}
