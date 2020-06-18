#include <iostream>
#include "shared_ptr.hpp"

using mini_stl::shared_ptr;
using std::cout;

struct deleter
{
    void operator ()(int *p)
    {
        cout << "destroying int at "
            << p << '\n';
        delete p;
    }
};

int main()
{
    shared_ptr<int> p1;
    shared_ptr<int> p2(new int(3));
    shared_ptr<int> p3(nullptr);
    shared_ptr<int> p4(new int(3), deleter());
    shared_ptr<int> p5(p4);
    shared_ptr<int> p6(std::move(p5));

	std::cout << "use_count:\n";
	std::cout << "p1: " << p1.use_count() << '\n';
	std::cout << "p2: " << p2.use_count() << '\n';
	std::cout << "p3: " << p3.use_count() << '\n';
	std::cout << "p4: " << p4.use_count() << '\n';
	std::cout << "p5: " << p5.use_count() << '\n';
	std::cout << "p6: " << p6.use_count() << '\n';

    return 0;
}
