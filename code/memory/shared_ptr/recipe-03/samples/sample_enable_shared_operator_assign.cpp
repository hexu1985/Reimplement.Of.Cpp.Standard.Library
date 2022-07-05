#include <memory>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "enable_shared_from_this.hpp"

using Hx::shared_ptr;
using Hx::make_shared;
using Hx::enable_shared_from_this;
 
class SharedInt : public enable_shared_from_this<SharedInt> {
public:
    explicit SharedInt(int n) : mNumber(n) {}
    SharedInt(const SharedInt&) = default;
    SharedInt(SharedInt&&) = default;
    ~SharedInt() = default;

    // 两个赋值运算符都使用 enable_shared_from_this::operator=
    SharedInt& operator=(const SharedInt&) = default;
    SharedInt& operator=(SharedInt&&) = default;

    int number() const { return mNumber; }

private:
    int mNumber;
};

int main() {
    shared_ptr<SharedInt> a = make_shared<SharedInt>(2);
    shared_ptr<SharedInt> b = make_shared<SharedInt>(4);
    *a = *b;

    std::cout << a->number() << std::endl;
}

/*
输出：

4
*/
