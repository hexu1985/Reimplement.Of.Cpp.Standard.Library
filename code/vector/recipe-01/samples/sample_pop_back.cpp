#include "vector.hpp"
#include <iostream>
 
template<typename T>
void print(T const & xs)
{
    std::cout << "[ ";
    for(auto const & x : xs) {
        std::cout << x << ' ';
    }
    std::cout << "]\n";
}
 
int main()
{
    Hx::vector<int> numbers;
 
    print(numbers); 
 
    numbers.push_back(5);
    numbers.push_back(3);
    numbers.push_back(4);
 
    print(numbers); 
 
    numbers.pop_back();
 
    print(numbers); 
}
