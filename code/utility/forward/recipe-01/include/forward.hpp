/**
 * @file forward.hpp
 * @brief 参考Effective Modern C++: 条款23
 *        以及g++头文件: c++/7.5.0/bits/move.h
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2022-04-08
 */
#ifndef MINI_STL_FORWARD_INC
#define MINI_STL_FORWARD_INC

#include <type_traits>

namespace Hx {

template<typename T>
T&& forward(typename std::remove_reference<T>::type& param)
{
    return static_cast<T&&>(param); 
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& param) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "template argument"
            " substituting T is an lvalue reference type");
    return static_cast<T&&>(param);
}

}   // namespace Hx

#endif
