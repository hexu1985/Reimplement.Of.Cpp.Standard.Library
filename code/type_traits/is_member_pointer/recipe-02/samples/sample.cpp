#include <iostream>
#include "is_member_pointer.hpp"
 
int main() {
    class cls {};
    std::cout << (Hx::is_member_pointer<int(cls::*)>::value
                     ? "T is member pointer"
                     : "T is not a member pointer") << '\n';
    std::cout << (Hx::is_member_pointer<int>::value
                     ? "T is member pointer"
                     : "T is not a member pointer") << '\n';
}
