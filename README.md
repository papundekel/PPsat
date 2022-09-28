# PPsat

A simple SAT solver.

Also contains programs `PPsat-queens-generate`, `PPsat-queens.sh`, `PPsat-crypt-generate` and `PPsat-crypt.sh` which make use of SAT/SMT solvers.

## Prerequisites

* C++2b capable compiler
* Bash
* grep, sed, gawk
* CMake >=3.20
* Ninja

### Solver

* ANTLR4, ANTLR C++ runtime, ANTLR C++ CMake packages
* Boost >=1.74

`-DGEN_SOLVER:BOOL=ON`

### Report

* Jupyter notebook

`-DGEN_REPORT:BOOL=ON`

#### Running the measurements

* Pin
* perf

## Configuration, build, install and run

`cmake -S ./ -B build/ -G "Ninja Multi-Config" [-DGEN_SOLVER:BOOL=ON] [-DGEN_REPORT:BOOL=ON]`

`cmake --build build/ --config Release --target all`

`mkdir -p install && cmake --install build/ --prefix install --config Release`

`install/bin/PPsat*`

### Report only

`make all`

Expects `PIN_ROOT` set to the root directory of Pin.

## Documentation

Each executable has a help option which prints its documentation.

`PPsat* -help`
