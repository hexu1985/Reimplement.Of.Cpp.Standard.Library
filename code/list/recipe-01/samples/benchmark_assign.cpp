#include <list>
#include <iostream>
#include <string>
 
int main()
{
    std::list<char> characters;
 
    auto print_list = [&](){
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';  
    };
 
    characters.assign(5, 'a');
    print_list();
 
    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_list();
 
    characters.assign({'C', '+', '+', '1', '1'});
    print_list();
}
