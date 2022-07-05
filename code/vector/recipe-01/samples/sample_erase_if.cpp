#include "vector.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>
#if __cplusplus >= 201703L
#include <string_view>
#else
#include <string>
#endif

#if __cplusplus <= 201703L
namespace std {

template< class T, class Alloc, class U >
typename Hx::vector<T,Alloc>::size_type erase(Hx::vector<T,Alloc>& c, const U& value)
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

template< class T, class Alloc, class Pred >
typename Hx::vector<T,Alloc>::size_type erase_if(Hx::vector<T,Alloc>& c, Pred pred)
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

}	// namespace std
#endif
 
#if __cplusplus >= 201703L
void print_container(std::string_view comment, const Hx::vector<char>& c)
#else
void print_container(const std::string& comment, const Hx::vector<char>& c)
#endif
{
    std::cout << comment;
    for (auto x : c) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}
 
int main()
{
    Hx::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    print_container("Init:\n", cnt);
 
    std::erase(cnt, '3');
    print_container("Erase '3':\n", cnt);
 
    auto erased = std::erase_if(cnt, [](char x) { return (x - '0') % 2 == 0; });
    print_container("Erase all even numbers:\n", cnt);
    std::cout << "In all " << erased << " even numbers were erased.\n";
}
