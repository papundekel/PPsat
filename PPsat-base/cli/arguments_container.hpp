#pragma once
#include <PPsat-base/cli/arguments.hpp>

#include <string_view>

namespace PPsat_base::cli
{
template <template <typename> typename C>
class arguments_container : public arguments
{
    C<std::string_view> storage;

    const std::string_view* begin() const noexcept override final
    {
        return std::data(storage);
    }
    const std::string_view* end() const noexcept override final
    {
        return std::data(storage) + storage.size();
    }

public:
    void add(const std::string_view argument) noexcept override final
    {
        storage.push_back(argument);
    }
};
}
