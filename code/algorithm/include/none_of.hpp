#pragma once

namespace Hx {

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last) {
        if (pred(*first)) return false;
        ++first;
    }
    return true;
}

}   // namespace Hx

