#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include "any.hpp"

#define PRINT(xxx) ( std::cout << std::setw(20) << std::left << (#xxx) << " = " << (xxx) << std::endl)


//检测一个any是否能转换成某种类型，RTTI
template<typename T>
bool can_cast_to(const Hx::any& a){
    return a.type() == typeid(T);
}

//获取any内部对象的引用，可以用来修改对象的值
template<typename T>
T& get_ref(Hx::any& a){
    assert(can_cast_to<T>(a) && "any类型转换错误！！！");
    return Hx::any_cast<T&>(a);
}

//获取原对象的指针
template<typename T>
T* get_ptr(Hx::any& a){
    assert(can_cast_to<T>(a) && "any类型转换错误！！！");
    return Hx::any_cast<T>(&a);
}



int main(int argc, char const *argv[])
{

/* 1.基本用法 */
    puts("1.基本用法");
    Hx::any a(1);
    //获取原对象   
    PRINT(Hx::any_cast<int>(a));
    
    //修改原对象的值,改为左值引用即可
    Hx::any_cast<int&>(a) = 100;
    PRINT(Hx::any_cast<int>(a));

    //获取原对象的指针
    int* ss = Hx::any_cast<int>(&a);
    PRINT(*ss);

    //获取原对象的类型信息
    PRINT(a.type().name());
    //检测any中是否保存了对象
    std::cout << std::boolalpha;
    PRINT(a.has_value());
    puts("");
/**************************************************************************/
/*二，智能指针。 不能用any保存堆上的原始的指针，会造成内存泄露,应使用智能指针*/

    puts("二，智能指针");
    //下面这样做会造成内存泄露
    int* p = new int(2);
    Hx::any ptr_any(p);
    int* tmp = Hx::any_cast<int*>(ptr_any);
    PRINT(*tmp);

    //应当使用智能指针，而且只能使用shared_ptr,因为scoped_ptr不能被复制
    //这样在any析构时，会调用shared_ptr的析构函数，释放其持有的资源
    Hx::any shared_any(std::shared_ptr<int>(new int(3) ) );
    auto p_shared = Hx::any_cast<std::shared_ptr<int> >(shared_any);
    PRINT(*p_shared);
    puts("");
/**************************************************************************/
//三，辅助函数：
    puts("三，辅助函数");
    std::string str("hello");
    Hx::any xxx(str);

    //检测一个any是否能转换成某种类型，RTTI
    PRINT(can_cast_to<int>(xxx) );
    PRINT(can_cast_to<std::string>(xxx) );

    //获取any内部对象的引用，可以用来修改对象的值
    get_ref<std::string>(xxx) = "world";

    //获取原对象的指针
    PRINT(get_ptr<std::string>(xxx)->size());
    puts("");
/**************************************************************************/
//四，用于容器:
    puts("四，用于容器");
    std::vector<Hx::any> vec {
        1,
        std::string("你好！"),
        1.414,    
        std::shared_ptr<std::string>(new std::string("end") )    
    };
    PRINT(Hx::any_cast<int>(vec[0]) );
    PRINT(Hx::any_cast<std::string&>(vec[1]) );
    PRINT(Hx::any_cast<double>(vec[2]) );
    PRINT(*Hx::any_cast<std::shared_ptr<std::string>>(vec[3]) );

    return 0;
}
