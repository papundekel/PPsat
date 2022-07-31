#pragma once

namespace PPsat
{
template <typename T, typename U, typename V>
const T& conditional(bool condition, const U& x, const V& y)
{
    using common_type = const std::common_type_t<T, U, V>&;
    return condition ? static_cast<common_type>(x)
                     : static_cast<common_type>(y);
}
}
