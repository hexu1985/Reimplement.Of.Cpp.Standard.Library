#include "list.hpp"
#include <iostream>
 
int main()
{ 
    Hx::list<int> nums {1, 3, 5, 7};
 
    std::cout << "nums contains " << nums.size() << " elements.\n";
}
