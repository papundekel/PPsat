#pragma once
#include <PPsat/variable.hpp>

#include <string>
#include <variant>

namespace PPsat
{
class variable_name_variant : public variable
{
protected:
    std::variant<std::string, std::size_t, std::monostate> name;

public:
    void print(std::ostream& output) const override final;

    std::string_view set_input_name(std::string&& name) override final;
    void set_input_name(std::size_t name) override final;
    bool has_input_name() const override final;
    std::size_t hash() const override final;
};

}
