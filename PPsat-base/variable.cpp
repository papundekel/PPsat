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

void PPsat_base::variable::assign_helper(void (clause::*memfn)(literal, bool),
                                         bool positive,
                                         std::function<void(clause&)> pre,
                                         std::function<void(clause&)> post)
{
    for_each_clause_containing(
        [this, memfn, positive, &pre, &post](clause& clause,
                                             bool positive_in_clause)
        {
            pre(clause);
            (clause.*memfn)({*this, positive}, positive_in_clause);
            post(clause);
        });
}

void PPsat_base::variable::assign(bool positive,
                                  std::function<void(clause&)> pre,
                                  std::function<void(clause&)> post)
{
    set_assignment(positive ? assignment::positive : assignment::negative);
    assign_helper(&clause::assign, positive, std::move(pre), std::move(post));
}

void PPsat_base::variable::unassign(bool positive,
                                    std::function<void(clause&)> pre,
                                    std::function<void(clause&)> post)
{
    set_assignment(assignment::unknown);
    assign_helper(&clause::unassign, positive, std::move(pre), std::move(post));
}
