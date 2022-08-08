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
    std::string_view rename(std::size_t name_native,
                            std::string&& name_input) override final;
    void rename(std::size_t name_native, std::size_t name_input) override final;

    std::optional<name> get(std::size_t name_internal) const override final;

    std::size_t count() const override final;
};

extern template class renaming_map<std::string>;
extern template class renaming_map<std::size_t>;
}
