#ifndef NONE_OF_INC
#define NONE_OF_INC

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last) {
        if (pred(*first)) return false;
        ++first;
    }
    return true;
}

#endif
