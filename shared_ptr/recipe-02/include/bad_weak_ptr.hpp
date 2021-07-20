/**
 * @file bad_weak_ptr.hpp
 * @brief 访问指向已销毁对象的 weak_ptr 时抛出的异常
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-06-16
 */
#ifndef MINI_STL_BAD_WEAK_PTR_INC
#define MINI_STL_BAD_WEAK_PTR_INC

#include <exception>

namespace mini_stl {

class bad_weak_ptr: public std::exception {
public:
    const char* what() const noexcept override;
};

}   // namespace mini_stl

#endif

