#pragma once

namespace PPsat
{
enum class variable_assignment
{
    positive,
    negative,
    unknown,
};

variable_assignment operator!(variable_assignment assignment);
}
