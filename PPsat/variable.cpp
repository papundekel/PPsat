#include <PPsat/clause.hpp>
#include <PPsat/unit.hpp>
#include <PPsat/variable.hpp>

std::string_view PPsat::variable::representation_set(std::string&& name)
{
    representation_set_impl();
    return representation_set_impl(std::move(name));
}
void PPsat::variable::representation_set(std::size_t name)
{
    representation_set_impl();
    representation_set_impl(name);
}

std::ostream& PPsat::operator<<(std::ostream& output, const variable& variable)
{
    variable.representation_print(output);
    return output;
}

std::tuple<PPsat_base::optional<PPsat::clause&>,
           std::list<PPsat::unit>,
           std::size_t>
PPsat::variable::assign(bool positive, std::size_t level, std::size_t recency)
{
    assignment_set(positive ? assignment::positive : assignment::negative);
    level_set(level);
    recency_set(recency);

    PPsat_base::optional<PPsat::clause&> conflict_anywhere;
    std::list<unit> units;

    auto count_visited = 0uz;

    for_each_clause_relevant_assign(
        [this, positive, &conflict_anywhere, &units, &count_visited](
            clause& clause,
            bool positive_in_clause)
        {
            ++count_visited;

            const auto [remove, conflict, literal_unit_opt] =
                clause.assign({*this, positive}, positive_in_clause);

            if (conflict)
            {
                conflict_anywhere = clause;
            }
            else if (literal_unit_opt)
            {
                units.emplace_back(*literal_unit_opt, clause);
            }

            return remove;
        });

    return {conflict_anywhere, std::move(units), count_visited};
}

void PPsat::variable::unassign(bool positive)
{
    assignment_set(assignment::unknown);
    antecedent_reset();
    for_each_clause_relevant_unassign(
        [this, positive](clause& clause, bool positive_in_clause)
        {
            clause.unassign({*this, positive}, positive_in_clause);
        });
}
