#include <iostream>
#include <set>
 
int main()
{
    std::set<int> example = {1, 2, 3, 4};
 
    for(int x: {2, 5}) {
        if(example.contains(x)) {
            std::cout << x << ": Found\n";
        } else {
            std::cout << x << ": Not found\n";
        }
    }
}

/*
Output:

2: Found
5: Not found
*/
