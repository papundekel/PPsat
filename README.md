# PPsat

A simple SAT solver.

Also contains programs `PPsat-queens-generate`, `PPsat-queens.sh`, `PPsat-crypt-generate` and `PPsat-crypt.sh` which make use of SAT/SMT solvers.

## Prerequisites

* C++2b capable compiler
* CMake >=3.20
* Ninja
* ANTLR4, ANTLR C++ runtime, ANTLR C++ CMake packages
* Boost >=1.74
* Bash
* grep, sed, gawk
* R
    * `rmarkdown`
    * `tidyverse`

## Configuration, build, install and run

`cmake -S ./ -B build/ -G "Ninja Multi-Config"`

`cmake --build build/ --config Release --target all`

`mkdir install && cmake --install build/ --prefix install --config Release`

`install/bin/PPsat*`

## Documentation

Each executable has a help option which prints its documentation.

`PPsat* -help`
