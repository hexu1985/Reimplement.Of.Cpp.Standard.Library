#include <iostream>
#include <memory>

using std::shared_ptr;
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

	std::cout << "use_count:\n";
	std::cout << "p1: " << p1.use_count() << '\n';
	std::cout << "p2: " << p2.use_count() << '\n';
	std::cout << "p3: " << p3.use_count() << '\n';
	std::cout << "p4: " << p4.use_count() << '\n';

    return 0;
}
