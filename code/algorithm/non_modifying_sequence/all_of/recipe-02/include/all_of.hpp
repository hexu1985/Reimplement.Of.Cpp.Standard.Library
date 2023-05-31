#pragma once

#include <algorithm>

namespace Hx {

template <typename InputIt, typename UnaryPredicate>
constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if_not(first, last, p) == last;
}

}   // namespace Hx

