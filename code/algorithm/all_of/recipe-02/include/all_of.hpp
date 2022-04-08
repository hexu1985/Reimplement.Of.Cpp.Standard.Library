#ifndef MINI_STL_ALL_OF_INC
#define MINI_STL_ALL_OF_INC

#include <algorithm>

namespace mini_stl {

template <class InputIt, class UnaryPredicate>
constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if_not(first, last, p) == last;
}

}   // namespace mini_stl

#endif
