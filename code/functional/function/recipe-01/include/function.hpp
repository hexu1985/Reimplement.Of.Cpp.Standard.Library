#pragma once

#include <utility>  // for std::forward
#include <type_traits>

namespace Hx {

template <typename R, typename... Args> 
class invoker_base {
public:
    virtual R operator()(Args... args)=0;
    virtual ~invoker_base() {}
};

template <typename R, typename... Args> 
class function_ptr_invoker : public invoker_base<R,Args...> {
    R (*func_)(Args...);

public:
    function_ptr_invoker(R (*func)(Args...)):func_(func) {}

    R operator()(Args... args) override {
        return (func_)(std::forward<Args>(args)...);
    }
};

namespace detail {

template <typename R, typename C, typename MT, typename T1, typename... Args>
R invoke_memptr(MT C::* memptr, T1&& t1, Args&&... args)
{
    if constexpr (std::is_function_v<MT>) {
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>) {
            return (std::forward<T1>(t1).*memptr)(std::forward<Args>(args)...);
        } else {
            return (std::forward<T1>(t1)->*memptr)(std::forward<Args>(args)...);
        }
    } else {
        static_assert(std::is_object_v<MT> && sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>) {
            return std::forward<T1>(t1).*memptr;
        } else {
            return std::forward<T1>(t1)->*memptr;
        }
    }
}

}   // namespace detail

template <typename R, typename MT, typename C, typename... Args>
class member_ptr_invoker : public invoker_base<R,Args...> {
    MT C::* memptr_;

public:
    member_ptr_invoker(MT C::* memptr):memptr_(memptr) {}

    R operator()(Args... args) override {
        return detail::invoke_memptr<R>(memptr_, std::forward<Args>(args)...);
    }
};

template <typename R, typename T, typename... Args> 
class function_object_invoker : public invoker_base<R,Args...> {
    T t_;

public:
    function_object_invoker(T t):t_(t) {}

    R operator()(Args... args) {
        return t_(std::forward<Args>(args)...);
    }
};

template <typename>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
    invoker_base<R,Args...>* invoker_;

public:
    using ResultType = R;

    function(R (*func)(Args...)) : 
        invoker_(new function_ptr_invoker<R,Args...>(func)) {}

    template <typename MT, typename C> function(MT C::* mptr) :
        invoker_(new member_ptr_invoker<R,MT,C,Args...>(mptr)) {}

    template <typename T> function(T t) : 
        invoker_(new function_object_invoker<R,T,Args...>(t)) {}

    R operator()(Args... args) {
        return (*invoker_)(std::forward<Args>(args)...);
    }

    ~function() {
        delete invoker_;
    }

};

}   // namespace Hx
