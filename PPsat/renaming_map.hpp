#pragma once
#include <PPsat/renaming.hpp>

#include <map>
#include <optional>

namespace PPsat
{
template <typename Value>
class renaming_map : public renaming
{
    std::map<std::size_t, Value> map;

public:
    void rename(std::string_view name_input,
                std::size_t name_internal) override final;
    void rename(std::size_t name_input,
                std::size_t name_internal) override final;

    std::optional<name> get(std::size_t name_internal) const override final;

    std::size_t count() const override final;
};

extern template class renaming_map<std::string_view>;
extern template class renaming_map<std::size_t>;
}
