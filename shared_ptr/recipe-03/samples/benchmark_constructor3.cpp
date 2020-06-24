// shared_ptr constructor example
#include <iostream>
#include <memory>

struct C {int* data;};

int main () {
    std::shared_ptr<int> p1;
    std::shared_ptr<int> p2 (nullptr);
    std::shared_ptr<int> p3 (new int);
    std::shared_ptr<int> p4 (new int, std::default_delete<int>());
    std::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
    std::shared_ptr<int> p6 (p5);
    std::shared_ptr<int> p7 (std::move(p6));
    std::shared_ptr<C> obj (new C);
    std::shared_ptr<int> p8 (obj, obj->data);

    std::cout << "use_count:\n";
    std::cout << "p1: " << p1.use_count() << '\n';
    std::cout << "p2: " << p2.use_count() << '\n';
    std::cout << "p3: " << p3.use_count() << '\n';
    std::cout << "p4: " << p4.use_count() << '\n';
    std::cout << "p5: " << p5.use_count() << '\n';
    std::cout << "p6: " << p6.use_count() << '\n';
    std::cout << "p7: " << p7.use_count() << '\n';
    std::cout << "p8: " << p8.use_count() << '\n';
    return 0;
}

/*
Output:
use_count:
p1: 0
p2: 0
p3: 1
p4: 1
p5: 2
p6: 0
p7: 2
p8: 2
*/
