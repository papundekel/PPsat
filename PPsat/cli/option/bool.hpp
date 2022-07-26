#pragma once
#include <cstddef>
#include <string_view>

namespace PPsat::cli::option
{
template <typename Base>
class bool_ : public Base
{
public:
    std::size_t argument_count() const noexcept override final
    {
        return 0;
    }

    bool parse(std::size_t, std::string_view) noexcept override final
    {
        return true;
    }
};
}
