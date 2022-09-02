# PPsat

A simple SAT solver.

Also contains programs `PPsat-queens-generate`, `PPsat-queens.sh`, `PPsat-crypt-generate` and `PPsat-crypt.sh` which make use of SAT/SMT solvers.

## Prerequisites

* Bash
* CMake >=3.20
* C++2b capable compiler

## Configuration, build, install and run

`cmake -S ./ -B build/`

`cmake --build build/ --config Release --target all`

`mkdir install && cmake build/ --prefix install --config Release`

`install/bin/PPsat*`

## Usage

Each executable has a help option which prints its documentation.

`PPsat* -help`
