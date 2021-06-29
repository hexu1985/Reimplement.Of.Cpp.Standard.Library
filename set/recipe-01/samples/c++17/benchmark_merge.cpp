#include <iostream>
#include <set>
 
// print out a container
template <class Os, class K>
Os& operator<<(Os& os, const std::set<K>& v) {
    os << '[' << v.size() << "] {";
    bool o{};
    for (const auto& e : v)
        os << (o ? ", " : (o = 1, " ")) << e;
    return os << " }\n";
}
 
int main()
{
    std::set<char>
        p{ 'C', 'B', 'B', 'A' }, 
        q{ 'E', 'D', 'E', 'C' };
 
    std::cout << "p: " << p << "q: " << q;
 
    p.merge(q);
 
    std::cout << "p.merge(q);\n" << "p: " << p << "q: " << q;
}

/*
Output:

p: [3] { A, B, C }
q: [3] { C, D, E }
p.merge(q);
p: [5] { A, B, C, D, E }
q: [1] { C }
*/
