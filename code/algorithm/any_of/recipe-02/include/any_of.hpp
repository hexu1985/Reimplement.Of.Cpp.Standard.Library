#ifndef MINI_STL_ANY_OF_INC
#define MINI_STL_ANY_OF_INC

#include <algorithm>

namespace Hx {

template< class InputIt, class UnaryPredicate >
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}

}   // namespace Hx

#endif
