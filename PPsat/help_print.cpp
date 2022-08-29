#include <PPsat/help_print.hpp>

#include <iostream>

void PPsat::help_print(std::ostream& out)
{
    out << "Usage:\n"
        << "\tPPsat [OPTION]... [INPUT [OUTPUT]]\n"
        << "\n"
        << "\tMissing file path means the program will use standard I/O.\n"
        << "\n"
        << "Options:\n"
        << "\t-help [<BOOL>]\t\t\t\t\tDisplays this help.\n"
        << "\t-subprogram [convert|solve]\t\t\tSubprogram selection.\n"
        << "\t-nnf [<BOOL>]\t\t\t\t\tThe input is in NNF.\n"
        << "\t-format [dimacs|smtlib]\t\t\t\tUse the specified format for the "
           "input.\n"
        << "\t-clause [basic|counting|watched_literals]\tThe type of data "
           "structure for unit propagation.\n"
        << "\t-adjacency [list|set|set_unordered]\t\tThe type of data "
           "structure for literal adjacency.\n"
        << "\t-decision [trivial|random|JW_static|VSIDS]\tThe type of "
           "decision heuristic.\n"
        << "\t-restart <DOUBLE>\t\t\t\tThe factor for the geometric restarts. "
           "Should "
           "be greater than 1.\n"
        << "\t-cdcl [<BOOL>]\t\t\t\t\tUse the CDCL version of the solver (as "
           "opposed "
           "to DPLL).\n"
        << "\t-assume [<BOOL>]\t\t\t\tAccept an assumption.\n";
}
