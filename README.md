# PPsat

A simple SAT solver.

Also contains programs `PPsat-queens-generate`, `PPsat-queens.sh`, `PPsat-crypt-generate` and `PPsat-crypt.sh` which make use of SAT/SMT solvers.

Contains a report about the solver performance and measurement scripts used to generate data used in the report.
The solver and the report can be built independently.

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

#### Running the report measurements

* Pin
* perf

## Configuration, build, install and run

### Report instrumentation tool only

This builds the Pin instrumentation tool used by the measurement scripts.
It is not necessary to build this to see the report.

`cd report`

`make all`

Expects `PIN_ROOT` set to the root directory of Pin.

### Report and solver

`cmake -S ./ -B build/ -G "Ninja Multi-Config" [-DGEN_SOLVER:BOOL=ON] [-DGEN_REPORT:BOOL=ON]`

`cmake --build build/ --config Release --target all`

`mkdir -p install && cmake --install build/ --prefix install --config Release`

`install/` contains the output files (executables, measurement scripts and the report).

## Documentation

Each executable has a help option which prints its documentation.

`PPsat* -help`
