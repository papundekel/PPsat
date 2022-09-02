#include <PPsat-queens/generate.hpp>

#include <array>
#include <iostream>

namespace
{
struct coord_sentinel
{
    friend class coord_t;

    std::size_t dimension;

public:
    coord_sentinel(std::size_t dimension) noexcept
        : dimension(dimension)
    {}
};

class coord_t
{
    std::size_t col_;
    std::size_t row_;

public:
    coord_t(std::size_t col, std::size_t row) noexcept
        : col_(col + 1)
        , row_(row + 1)
    {}

    auto col() const noexcept
    {
        return col_ - 1;
    }
    auto row() const noexcept
    {
        return row_ - 1;
    }

    auto to_var(std::size_t dimension) const noexcept
    {
        return row() * dimension + col() + 1;
    }

    bool operator==(coord_sentinel sentinel) const noexcept
    {
        return col_ == 0uz || row_ == 0uz || col() == sentinel.dimension ||
               row() == sentinel.dimension;
    }
};

void print_header(std::size_t dimension, std::ostream& out)
{
    auto count_piars = dimension * (dimension - 1uz) / 2uz;

    auto count_variable = dimension * dimension;

    auto per_row_at_least = 1uz;
    auto per_row = count_piars;
    auto per_col = count_piars;
    auto per_diag_big = count_piars;
    auto per_diag_triangle =
        dimension * (dimension - 1uz) * (dimension - 2uz) / 6uz;

    auto count_clause = dimension * (per_row_at_least + per_row + per_col) +
                        2uz * per_diag_big + 4uz * per_diag_triangle;

    out << "c N queens problem for N = " << dimension << "\n"
        << "p cnf " << count_variable << " " << count_clause << "\n";
}

void print_line(auto dimension, auto begin, auto direction, std::ostream& out)
{
    for (auto i = begin; i != coord_sentinel(dimension); i = direction(i))
    {
        out << i.to_var(dimension) << " ";
    }

    out << "0\n";
}

void print_pairs(std::size_t dimension,
                 coord_t begin,
                 auto direction,
                 std::ostream& out)
{
    for (auto i = begin; i != coord_sentinel(dimension); i = direction(i))
    {
        for (auto j = direction(i); j != coord_sentinel(dimension);
             j = direction(j))
        {
            out << "-" << i.to_var(dimension) << " -" << j.to_var(dimension)
                << " 0\n";
        }
    }
}

void print_rows_at_least(std::size_t dimension, std::ostream& out)
{
    for (auto row = 0uz; row != dimension; ++row)
    {
        print_line(
            dimension,
            coord_t(0, row),
            [](coord_t coord)
            {
                return coord_t(coord.col() + 1, coord.row());
            },
            out);
    }
}

void print_rows(std::size_t dimension, std::ostream& out)
{
    for (auto row = 0uz; row != dimension; ++row)
    {
        print_pairs(
            dimension,
            coord_t(0uz, row),
            [](coord_t coord)
            {
                return coord_t(coord.col() + 1, coord.row());
            },
            out);
    }
}

void print_cols(std::size_t dimension, std::ostream& out)
{
    for (auto col = 0uz; col != dimension; ++col)
    {
        print_pairs(
            dimension,
            coord_t(col, 0uz),
            [](coord_t coord)
            {
                return coord_t(coord.col(), coord.row() + 1);
            },
            out);
    }
}

void print_diags(std::size_t dimension, std::ostream& out)
{
    if (dimension <= 1)
    {
        return;
    }

    auto print_pairs_diag = [dimension, &out](auto begin, auto direction)
    {
        print_pairs(dimension, begin, direction, out);
    };

    auto print_pairs_diag_down = [print_pairs_diag](auto begin)
    {
        print_pairs_diag(begin,
                         [](coord_t coord)
                         {
                             return coord_t(coord.col() + 1, coord.row() + 1);
                         });
    };

    auto print_pairs_diag_up = [print_pairs_diag](auto begin)
    {
        print_pairs_diag(begin,
                         [](coord_t coord)
                         {
                             return coord_t(coord.col() + 1, coord.row() - 1);
                         });
    };

    for (auto col = 0uz; col != dimension; ++col)
    {
        print_pairs_diag_down(coord_t(col, 0));
        print_pairs_diag_up(coord_t(col, dimension - 1));
    }

    for (auto row = 1uz; row != dimension - 1uz; ++row)
    {
        print_pairs_diag_down(coord_t(0uz, row));
        print_pairs_diag_up(coord_t(0uz, row));
    }
}
}

void PPsat_queens::generate(std::size_t dimension, std::ostream& output)
{
    static constexpr std::array printers = {print_header,
                                            print_rows_at_least,
                                            print_rows,
                                            print_cols,
                                            print_diags};

    for (auto printer : printers)
    {
        printer(dimension, output);
    }
}
