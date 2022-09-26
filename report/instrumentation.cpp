#include <pin.H>

#include <iostream>

static std::size_t counter = 0;

void increment_counter()
{
    ++counter;
}

void f(INS ins, VOID* v)
{
    if (INS_IsProcedureCall(ins) && INS_IsIndirectControlFlow(ins))
    {
        INS_InsertCall(ins,
                       IPOINT_BEFORE,
                       (AFUNPTR)increment_counter,
                       IARG_END);
    }
}

void Fini(INT32, void*)
{
    std::cout << counter;
}

int main(int argc, char* argv[])
{
    if (PIN_Init(argc, argv))
    {
        return 1;
    }

    INS_AddInstrumentFunction(f, nullptr);

    PIN_AddFiniFunction(Fini, nullptr);

    PIN_StartProgram();

    return 0;
}
