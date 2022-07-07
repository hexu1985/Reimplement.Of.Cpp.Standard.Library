/**
 * @file move.hpp
 * @brief 参考Effective Modern C++: 条款23
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2022-04-08
 */
#ifndef MINI_STL_MOVE_INC
#define MINI_STL_MOVE_INC

#include <type_traits>

namespace Hx {

template <typename T>
typename std::remove_reference<T>::type&&
move(T&& param)
{
    using ReturnType =
        typename std::remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

}   // namespace Hx

#endif
