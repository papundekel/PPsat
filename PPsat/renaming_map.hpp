#pragma once
#include <PPsat/renaming.hpp>

namespace PPsat
{
class renaming_map : public renaming
{
    std::map<std::size_t, std::string_view> map;

public:
    void rename(std::string_view name_input,
                std::size_t name_internal) override final
    {
        map.try_emplace(name_internal, name_input);
    }

    std::optional<std::string_view> get(std::size_t name_internal) const
    {
        auto i = map.find(name_internal);

        if (i == map.end())
        {
            return {};
        }

        return i->second;
    }

    void for_each(std::function<void(std::size_t, std::string_view)> f)
        const override final
    {
        std::ranges::for_each(map,
                              [&f](const auto& t)
                              {
                                  f(t.first, t.second);
                              });
    }

    std::size_t count() const
    {
        return map.size();
    }
};
}
