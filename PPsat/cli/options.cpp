#include "PPsat/subprogram.hpp"
#include <PPsat/cli/options.hpp>

#include <PPsat/clause_type.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/view_any.hpp>
#include <tuple>

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
          {"restart"},
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
               std::make_pair("vector", adjacency_type::vector),
               std::make_pair("list", adjacency_type::list),
               std::make_pair("set", adjacency_type::set),
               std::make_pair("set_unordered", adjacency_type::set_unordered)}}}
{}
