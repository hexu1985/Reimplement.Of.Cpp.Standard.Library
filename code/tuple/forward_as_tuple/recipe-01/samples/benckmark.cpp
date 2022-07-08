// forward_as_tuple example
#include <iostream>     // std::cout
#include <tuple>        // std::tuple, std::get, std::forward_as_tuple
#include <string>       // std::string

void print_pack (std::tuple<std::string&&,int&&> pack) {
    std::cout << std::get<0>(pack) << ", " << std::get<1>(pack) << '\n';
}

int main() {
    std::string str ("John");
    print_pack (std::forward_as_tuple(str+" Smith",25));
    print_pack (std::forward_as_tuple(str+" Daniels",22));
    return 0;
}
