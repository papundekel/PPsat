#pragma once
#include <PPsat-base/clause.hpp>

namespace PPsat
{
class clause_no_add : public PPsat_base::clause
{
    void add_literal(PPsat_base::literal literal) override final;
};
}
