#ifndef ALL_OF_INC
#define ALL_OF_INC

#include <algorithm>

template <class InputIt, class UnaryPredicate>
constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if_not(first, last, p) == last;
}

#endif
