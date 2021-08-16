#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <any>

bool is_int(const std::any& a) {
    return typeid(int)==a.type();
}

bool is_string(const std::any& a) {
    return typeid(std::string)==a.type();
}

template <typename T> bool contains (const std::any& a) {
    return typeid(T)==a.type();
}

class any_counter {
    int count_;

public:
    any_counter() : count_(0) {}

    int operator()(const std::any& a) {
        return a.has_value() ? ++count_ : count_;
    }

    int count() const { return count_; }
};

template <typename OutIt,typename Type> class extractor {
    OutIt it_;

public:
    extractor(OutIt it) : it_(it) {}

    void operator()(std::any& a) {
        Type* t(std::any_cast<Type>(&a));
        if (t) {
            *it_++ = *t;
        }
    }
};

template <typename Type, typename OutIt>
extractor<OutIt,Type> make_extractor(OutIt it) {
    return extractor<OutIt,Type>(it);
}

int main() {
    std::cout << "Example of using predicates and the "
        "function object any_counter\n";

    std::vector<std::any> vec;
    vec.push_back(std::any());
    for(int i=0;i<10;++i) {
        vec.push_back(i);
    }
    vec.push_back(std::any());

    // 计算含有值的any 实例的数量
    int i=std::for_each(
            vec.begin(),
            vec.end(),
            any_counter()).count();
    std::cout
        << "There are " << i << " non-empty any's in vec\n\n";

    // 从vec 中取出所有int
    std::list<int> lst;
    std::for_each(vec.begin(),vec.end(),
            make_extractor<int>(std::back_inserter(lst)));
    std::cout << "Found " << lst.size() << " ints in vec\n\n";

    vec.clear();

    vec.push_back(std::string("This is a string"));
    vec.push_back(42);
    vec.push_back(3.14);

    if (is_string(vec[0])) {
        std::cout << "Found me a string!\n";
    }

    if (is_int(vec[1])) {
        std::cout << "Found me an int!\n";
    }

    if (contains<double>(vec[2])) {
        std::cout <<
            "The generic tool is sweeter, found me a double!\n";
    }
}


