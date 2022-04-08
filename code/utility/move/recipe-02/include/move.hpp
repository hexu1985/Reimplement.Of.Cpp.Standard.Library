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

namespace mini_stl {

template<typename T> // C++14; still in
decltype(auto) move(T&& param)
{
    using ReturnType = std::remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

}   // namespace mini_stl

#endif