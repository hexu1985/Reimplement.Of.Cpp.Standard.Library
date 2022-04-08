#ifndef MINI_STL_NONE_OF_INC
#define MINI_STL_NONE_OF_INC

namespace mini_stl {

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last) {
        if (pred(*first)) return false;
        ++first;
    }
    return true;
}

}   // namespace mini_stl

#endif
