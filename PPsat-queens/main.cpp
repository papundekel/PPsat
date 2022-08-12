#include <array>
#include <charconv>
#include <cstring>
#include <iostream>

namespace
{
template <typename T>
struct coord_sentinel
{
    T dimension;
};

template <typename T>
class coord_t
{
    T col_;
    T row_;

public:
    coord_t(T col, T row) noexcept
        : col_(col + 1)
        , row_(row + 1)
    {}

    bool operator==(coord_sentinel<T> sentinel) const noexcept
    {
        return col_ == 0 || row_ == 0 || col() == sentinel.dimension ||
               row() == sentinel.dimension;
    }

    auto col() const noexcept
    {
        return col_ - 1;
    }

    auto row() const noexcept
    {
        return row_ - 1;
    }
    auto to_var(T dimension) const noexcept
    {
        return row() * dimension + col() + 1;
    }
};
template <typename T>
coord_t(T, int) -> coord_t<T>;
template <typename T>
coord_t(int, T) -> coord_t<T>;

void print_header(unsigned int dimension, std::ostream& out)
{
    auto count_piars = dimension * (dimension - 1) / 2;

    auto count_variable = dimension * dimension;

    auto per_row_at_least = 1;
    auto per_row = count_piars;
    auto per_col = count_piars;
    auto per_diag_big = count_piars;
    auto per_diag_triangle = dimension * (dimension - 1) * (dimension - 2) / 6;

    auto count_clause = dimension * (per_row_at_least + per_row + per_col) +
                        2 * per_diag_big + 4 * per_diag_triangle;

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

void print_pairs(auto dimension, auto begin, auto direction, std::ostream& out)
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

void print_rows_at_least(unsigned int dimension, std::ostream& out)
{
    for (auto row = decltype(dimension)(0); row != dimension; ++row)
    {
        print_line(
            dimension,
            coord_t{0, row},
            [](auto coord)
            {
                return coord_t{coord.col() + 1, coord.row()};
            },
            out);
    }
}

void print_rows(unsigned int dimension, std::ostream& out)
{
    for (auto row = decltype(dimension)(0); row != dimension; ++row)
    {
        print_pairs(
            dimension,
            coord_t{0, row},
            [](auto coord)
            {
                return coord_t{coord.col() + 1, coord.row()};
            },
            out);
    }
}

void print_cols(unsigned int dimension, std::ostream& out)
{
    for (auto col = decltype(dimension)(0); col != dimension; ++col)
    {
        print_pairs(
            dimension,
            coord_t{col, 0},
            [](auto coord)
            {
                return coord_t{coord.col(), coord.row() + 1};
            },
            out);
    }
}

void print_diags(unsigned int dimension, std::ostream& out)
{
    if (dimension == 1)
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
                         [](auto coord)
                         {
                             return coord_t{coord.col() + 1, coord.row() + 1};
                         });
    };

    auto print_pairs_diag_up = [print_pairs_diag](auto begin)
    {
        print_pairs_diag(begin,
                         [](auto coord)
                         {
                             return coord_t{coord.col() + 1, coord.row() - 1};
                         });
    };

    for (auto col = decltype(dimension)(0); col != dimension; ++col)
    {
        print_pairs_diag_down(coord_t{col, 0});
        print_pairs_diag_up(coord_t{col, dimension - 1});
    }

    for (auto row = decltype(dimension)(1); row != dimension - 1; ++row)
    {
        print_pairs_diag_down(coord_t{0, row});
        print_pairs_diag_up(coord_t{0, row});
    }
}

void PPsat_queens(unsigned int dimension)
{
    constexpr std::array printers = {print_header,
                                     print_rows_at_least,
                                     print_rows,
                                     print_cols,
                                     print_diags};

    for (auto printer : printers)
    {
        printer(dimension, std::cout);
    }
}
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Invalid argument count.\n";
        return 1;
    }

    auto dimension_str_len = std::strlen(argv[1]);

    unsigned int dimension;
    auto parse_result =
        std::from_chars(argv[1], argv[1] + dimension_str_len, dimension);

    if (parse_result.ec != std::errc())
    {
        std::cerr << "Argument parse error.\n";
        return 2;
    }

    PPsat_queens(dimension);

    return 0;
}
