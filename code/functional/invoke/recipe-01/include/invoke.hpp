#pragma once

#include <type_traits>

namespace Hx {

namespace detail {

template <typename>
constexpr bool is_reference_wrapper_v = false;

template <typename U>
constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;
 
template <typename C, typename Pointed, typename T1, typename... Args>
constexpr decltype(auto) invoke_memptr(Pointed C::* f, T1&& t1, Args&&... args)
{
    if constexpr (std::is_function_v<Pointed>) {
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
            return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
        else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
            return (t1.get().*f)(std::forward<Args>(args)...);
        else
            return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
    } else {
        static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
            return std::forward<T1>(t1).*f;
        else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
            return t1.get().*f;
        else
            return (*std::forward<T1>(t1)).*f;
    }
}

} // namespace detail
 
template <typename F, typename... Args>
constexpr std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) 
    noexcept(std::is_nothrow_invocable_v<F, Args...>)
{
    if constexpr (std::is_member_pointer_v<std::decay_t<F>>)
        return detail::invoke_memptr(f, std::forward<Args>(args)...);
    else
        return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename R, typename F, typename... Args>
    // requires std::is_invocable_r_v<R, F, Args...>
constexpr R invoke_r(F&& f, Args&&... args)
    noexcept(std::is_nothrow_invocable_r_v<R, F, Args...>)
{
    if constexpr (std::is_void_v<R>)
        invoke(std::forward<F>(f), std::forward<Args>(args)...);
    else
        return invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

}   // namespace Hx

#ifndef __cpp_lib_invoke_r
#define __cpp_lib_invoke_r
#endif

