#pragma once

namespace Hx {

template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first!=last) {
        if (pred(*first)) return true;
        ++first;
    }
    return false;
}

}   // namespace Hx

