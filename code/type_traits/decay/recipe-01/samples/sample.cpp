#include <type_traits>
#include "decay.hpp"
 
template <typename T, typename U>
struct decay_equiv : 
    std::is_same<typename Hx::decay<T>::type, U>::type 
{};
template <typename T, typename U>
constexpr bool is_decay_equ = decay_equiv<T, U>::value;
 
int main()
{
    static_assert(
        is_decay_equ<int, int> &&
        ! is_decay_equ<int, float> &&
        is_decay_equ<int&, int> &&
        is_decay_equ<int&&, int> &&
        is_decay_equ<const int&, int> &&
        is_decay_equ<int[2], int*> &&
        ! is_decay_equ<int[4][2], int*> &&
        ! is_decay_equ<int[4][2], int**> &&
        is_decay_equ<int[4][2], int(*)[2]> &&
        is_decay_equ<int(int), int(*)(int)>
    );
}
