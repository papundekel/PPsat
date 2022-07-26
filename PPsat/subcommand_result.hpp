#pragma once

namespace PPsat
{
class subcommand_result
{
    int return_code;
    bool success;

public:
    subcommand_result() noexcept
        : return_code()
        , success(false)
    {}

    subcommand_result(int return_code) noexcept
        : return_code(return_code)
        , success(true)
    {}

    operator bool() const
    {
        return success;
    }

    int code() const
    {
        return return_code;
    }
};
}
