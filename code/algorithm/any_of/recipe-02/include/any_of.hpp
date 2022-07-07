#pragma once

#include <algorithm>

namespace Hx {

template <typename InputIt, typename UnaryPredicate>
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}

}   // namespace Hx

