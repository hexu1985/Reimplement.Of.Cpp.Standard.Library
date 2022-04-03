#ifndef NONE_OF_INC
#define NONE_OF_INC

#include <algorithm>

template< class InputIt, class UnaryPredicate >
constexpr bool none_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) == last;
}

#endif
