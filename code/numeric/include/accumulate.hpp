#pragma once

namespace Hx {

template <typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
    for ( ; first != last; ++first)
        init = init + *first;
    return init;
}

template <typename InputIterator, typename T, typename BinaryOperatiron>
T accumulate(InputIterator first, InputIterator last, T init,
        BinaryOperatiron binary_op)
{
    for ( ; first != last; ++first)
        init = binary_op(init, *first);
    return init;
}

}   // Hx

