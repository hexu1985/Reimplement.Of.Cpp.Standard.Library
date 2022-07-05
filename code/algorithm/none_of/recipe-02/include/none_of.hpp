#ifndef MINI_STL_NONE_OF_INC
#define MINI_STL_NONE_OF_INC

#include <algorithm>

namespace Hx {

template< class InputIt, class UnaryPredicate >
constexpr bool none_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) == last;
}

}   // namespace Hx

#endif
