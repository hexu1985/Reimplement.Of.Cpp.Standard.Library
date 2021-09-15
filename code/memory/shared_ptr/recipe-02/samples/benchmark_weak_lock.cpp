// weak_ptr::lock example
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

int main () {
    shared_ptr<int> sp1,sp2;
    weak_ptr<int> wp;
                                        // sharing group:
                                        // --------------
    sp1 = make_shared<int> (20);        // sp1
    wp = sp1;                           // sp1, wp

    sp2 = wp.lock();                    // sp1, wp, sp2
    sp1.reset();                        //      wp, sp2

    sp1 = wp.lock();                    // sp1, wp, sp2

    std::cout << "*sp1: " << *sp1 << '\n';
    std::cout << "*sp2: " << *sp2 << '\n';

    return 0;
}


/*
Output:

*sp1: 20
*sp2: 20
*/
