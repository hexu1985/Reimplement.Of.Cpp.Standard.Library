#include "list.hpp"
#include <iostream>
 
int main()
{ 
    mini_stl::list<int> nums {1, 3, 5, 7};
 
    std::cout << "nums contains " << nums.size() << " elements.\n";
}
