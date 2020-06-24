// weak_ptr constructor example
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;

struct C {int* data;};

int main () 
{
    shared_ptr<int> sp (new int);

    weak_ptr<int> wp1;
    weak_ptr<int> wp2 (wp1);
    weak_ptr<int> wp3 (sp);
    weak_ptr<int> wp4 (std::move(wp3));

    std::cout << "use_count:\n";
    std::cout << "wp1: " << wp1.use_count() << '\n';
    std::cout << "wp2: " << wp2.use_count() << '\n';
    std::cout << "wp3: " << wp3.use_count() << '\n';
    std::cout << "wp4: " << wp4.use_count() << '\n';
    return 0;
}


/*
Output:

use_count:
wp1: 0
wp2: 0
wp3: 0
wp4: 1
*/
