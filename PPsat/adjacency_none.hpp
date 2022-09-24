#pragma once
#include <PPsat/adjacency.hpp>
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

namespace PPsat
{
class adjacency_none : public virtual adjacency
{
public:
    void adjacent_for_each(std::function<bool(clause&, bool)>) override final
    {}

    void adjacent_add(clause&, bool) override final
    {}

    void adjacent_remove(clause&, bool) override final
    {}

    std::size_t size() const override final
    {
        return 0;
    }

    void adjacent_reset() override final
    {}
};
}
