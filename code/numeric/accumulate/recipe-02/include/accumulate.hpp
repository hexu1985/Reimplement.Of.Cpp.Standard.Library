#ifndef ACCUMULATE_INC
#define ACCUMULATE_INC

template <typename InputIt, typename T>
constexpr // since C++20
T accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first) {
        init = std::move(init) + *first; // std::move since C++20
    }
    return init;
}

template <typename InputIt, typename T, typename BinaryOperation>
constexpr // since C++20
T accumulate(InputIt first, InputIt last, T init,
        BinaryOperation op)
{
    for (; first != last; ++first) {
        init = op(std::move(init), *first); // std::move since C++20
    }
    return init;
}

#endif

