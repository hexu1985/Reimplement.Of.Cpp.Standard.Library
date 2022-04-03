#ifndef ANY_OF_INC
#define ANY_OF_INC

template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first!=last) {
        if (pred(*first)) return true;
        ++first;
    }
    return false;
}

#endif
