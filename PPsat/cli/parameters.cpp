#include <PPsat/cli/parameters.hpp>

#include <PPsat/subprogram.hpp>

#include <PPsat-base/view_any.hpp>

#include <array>
#include <random>
#include <tuple>
#include <utility>

PPsat::cli::parameters::parameters()
    : parameters_base_options{{false, "iterations", 1},
                              {false, "random", std::random_device()()},
                              {false, "restart", 2.5},
                              {false, "help", false},
                              {false, "nnf", false},
                              {false, "assume", false},
                              {false,
                               "output",
                               output_format::human_readable,
                               std::array{
                                   std::make_pair(
                                       "human_readable",
                                       output_format::human_readable),
                                   std::make_pair("csv", output_format::csv),
                                   std::make_pair("none",
                                                  output_format::none)}},
                              {true,
                               "subprogram",
                               subprogram::selection::solve,
                               std::array{std::make_pair(
                                              "convert",
                                              subprogram::selection::convert),
                                          std::make_pair(
                                              "solve",
                                              subprogram::selection::solve)}},
                              {false,
                               "decision",
                               decision::type::JW_static,
                               std::array{
                                   std::make_pair("trivial",
                                                  decision::type::trivial),
                                   std::make_pair(
                                       "deterministic",
                                       decision::type::deterministic),
                                   std::make_pair("random",
                                                  decision::type::random),
                                   std::make_pair("JW_static",
                                                  decision::type::JW_static),
                                   std::make_pair("VSIDS",
                                                  decision::type::VSIDS)}},
                              {false,
                               "format",
                               formula_format::unspecified,
                               std::array{
                                   std::make_pair("dimacs",
                                                  formula_format::DIMACS),
                                   std::make_pair("smtlib",
                                                  formula_format::SMTLIB)}},
                              {false,
                               "clause",
                               clause::type::watched_literals,
                               std::array{
                                   std::make_pair("basic", clause::type::basic),
                                   std::make_pair("counting",
                                                  clause::type::counting),
                                   std::make_pair(
                                       "watched_literals",
                                       clause::type::watched_literals)}},
                              {false,
                               "adjacency",
                               adjacency::type::vector,
                               std::array{
                                   std::make_pair("vector",
                                                  adjacency::type::vector),
                                   std::make_pair("list",
                                                  adjacency::type::list),
                                   std::make_pair("set", adjacency::type::set),
                                   std::make_pair(
                                       "set_unordered",
                                       adjacency::type::set_unordered)}},
                              {false, "virtual", false},
                              {false, "cdcl", true}}
    , parameters_base_arguments()
{}
