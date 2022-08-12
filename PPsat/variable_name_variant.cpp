#include <PPsat/variable_name_variant.hpp>

#include <iostream>

namespace
{
template <typename... T>
struct overloaded : T...
{
    using T::operator()...;
};
}

void PPsat::variable_name_variant::print(std::ostream& output) const
{
    std::visit(overloaded{[](std::monostate) {},
                          [&output](const auto& name)
                          {
                              output << name;
                          }},
               name);
}

std::string_view PPsat::variable_name_variant::set_input_name(
    std::string&& name_new)
{
    auto& str = name.emplace<std::string>(std::move(name_new));
    return str;
}

void PPsat::variable_name_variant::set_input_name(std::size_t name_new)
{
    name.emplace<std::size_t>(name_new);
}

bool PPsat::variable_name_variant::has_input_name() const
{
    return std::visit(overloaded{[](std::monostate)
                                 {
                                     return false;
                                 },
                                 [](auto&&)
                                 {
                                     return true;
                                 }},
                      name);
}
std::size_t PPsat::variable_name_variant::hash() const
{
    return std::visit(overloaded{[](std::size_t name)
                                 {
                                     return name;
                                 },
                                 [](auto&&)
                                 {
                                     return 0uz;
                                 }},
                      name);
}
