#include <iostream>
#include <type_traits>
#include "remove_cv.hpp"
 
template<typename U, typename V> constexpr bool same = std::is_same_v<U, V>;
 
static_assert(
        same< Hx::remove_cv_t< int >, int >
    and same< Hx::remove_cv_t< const int >, int >
    and same< Hx::remove_cv_t< volatile int >, int >
    and same< Hx::remove_cv_t< const volatile int >, int >
    and same< Hx::remove_cv_t< const volatile int* >, const volatile int* >
    and same< Hx::remove_cv_t< const int* volatile >, const int* >
    and same< Hx::remove_cv_t< int* const volatile >, int* >
);
 
int main() {
    std::cout << std::boolalpha;
 
    using type1 = Hx::remove_cv<const int>::type;
    using type2 = Hx::remove_cv<volatile int>::type;
    using type3 = Hx::remove_cv<const volatile int>::type;
    using type4 = Hx::remove_cv<const volatile int*>::type;
    using type5 = Hx::remove_cv<int* const volatile>::type;
 
    std::cout << std::is_same<type1, int>::value << "\n";
    std::cout << std::is_same<type2, int>::value << "\n";
    std::cout << std::is_same<type3, int>::value << "\n";
    std::cout << std::is_same<type4, int*>::value << " "
              << std::is_same<type4, const volatile int*>::value << "\n";
    std::cout << std::is_same<type5, int*>::value << "\n";
}
