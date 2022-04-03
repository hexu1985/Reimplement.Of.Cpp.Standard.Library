#ifndef ANY_OF_INC
#define ANY_OF_INC

#include <algorithm>

template< class InputIt, class UnaryPredicate >
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}

#endif
