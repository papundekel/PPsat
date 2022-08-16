#include "PPsat-base/variable.hpp"
#include <PPsat-crypt/variable.hpp>

void PPsat_crypt::variable::representation_print(std::ostream& output) const
{}
std::string_view PPsat_crypt::variable::representation_set_impl(
    std::string&& name)
{
    return "";
}
void PPsat_crypt::variable::representation_set_impl(std::size_t name)
{}
void PPsat_crypt::variable::representation_set_impl()
{}
std::string_view PPsat_crypt::variable::representation_get_string() const
{
    return "";
}
std::size_t PPsat_crypt::variable::representation_get_int() const
{
    return 0;
}
bool PPsat_crypt::variable::representation_has() const
{
    return false;
}
std::size_t PPsat_crypt::variable::representation_hash() const
{
    return 0;
}
void PPsat_crypt::variable::set_assignment(assignment assignment)
{}
PPsat_base::variable::assignment PPsat_crypt::variable::get_assignment() const
{
    return assignment::unknown;
}
void PPsat_crypt::variable::for_each_clause_containing(
    std::function<void(PPsat_base::clause&, bool)> f) const
{}
void PPsat_crypt::variable::register_containing_clause(
    PPsat_base::clause& clause,
    bool positive)
{}
