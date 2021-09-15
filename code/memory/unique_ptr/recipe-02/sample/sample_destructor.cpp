#include <iostream>
#include "unique_ptr.hpp"
 
int main () 
{
    auto deleter = [](int* ptr){
        std::cout << "[deleter called]\n";
        delete ptr;
    };
 
    mini_stl::unique_ptr<int,decltype(deleter)> uniq(new int, deleter);
    std::cout << (uniq ? "not empty\n" : "empty\n");
    uniq.reset();
    std::cout << (uniq ? "not empty\n" : "empty\n");
}

/*
Output:

not empty
[deleter called]
empty
*/
