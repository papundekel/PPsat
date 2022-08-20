#include <PPsat-base/clause.hpp>
#include <PPsat-base/variable.hpp>

std::string_view PPsat_base::variable::representation_set(std::string&& name)
{
    representation_set_impl();
    return representation_set_impl(std::move(name));
}
void PPsat_base::variable::representation_set(std::size_t name)
{
    representation_set_impl();
    representation_set_impl(name);
}

std::ostream& PPsat_base::operator<<(std::ostream& output,
                                     const variable& variable)
{
    variable.representation_print(output);
    return output;
}

std::tuple<bool, std::optional<PPsat_base::literal>, std::size_t>
PPsat_base::variable::assign(bool positive)
{
    set_assignment(positive ? assignment::positive : assignment::negative);

    auto conflict_anywhere = false;
    std::optional<PPsat_base::literal> literal_unit_anywhere_opt;

    auto count_visited = 0uz;

    for_each_clause_relevant_assign(
        [this,
         positive,
         &conflict_anywhere,
         &literal_unit_anywhere_opt,
         &count_visited](clause& clause, bool positive_in_clause)
        {
            ++count_visited;

            const auto [conflict, literal_unit_opt] =
                clause.assign({*this, positive}, positive_in_clause);

            if (conflict)
            {
                conflict_anywhere = true;
            }
            else if (literal_unit_opt)
            {
                literal_unit_anywhere_opt = literal_unit_opt;
            }
        });

    return {conflict_anywhere, literal_unit_anywhere_opt, count_visited};
}

void PPsat_base::variable::unassign(bool positive)
{
    set_assignment(assignment::unknown);
    for_each_clause_relevant_unassign(
        [this, positive](clause& clause, bool positive_in_clause)
        {
            clause.unassign({*this, positive}, positive_in_clause);
        });
}
