#pragma once

#include <type_traits>

namespace Hx {

template <typename R, typename Arg> 
class invoker_base {
public:
    virtual R operator()(Arg arg)=0;
    virtual ~invoker_base() {}
};

template <typename R, typename Arg> 
class function_ptr_invoker : public invoker_base<R,Arg> {
    R (*func_)(Arg);

public:
    function_ptr_invoker(R (*func)(Arg)):func_(func) {}

    R operator()(Arg arg) {
        return (func_)(arg);
    }
};

namespace detail {

template <typename R, typename Arg, typename C, typename MT>
R invoke_memptr(MT C::* memptr, Arg&& arg)
{
    if constexpr (std::is_base_of_v<C, std::decay_t<Arg>>) {    // object
        return (arg.*memptr)();
    } else {
        return (arg->*memptr)();
    }
}

}   // namespace detail

template <typename R, typename Arg, typename C, typename MT> 
class member_ptr_invoker : public invoker_base<R,Arg> {
    MT C::* memptr_;

public:
    member_ptr_invoker(MT C::* memptr): memptr_(memptr) {}

    R operator()(Arg arg) {
        return detail::invoke_memptr<R>(memptr_, arg);
    }
};

template <typename R, typename Arg, typename T> 
class function_object_invoker : public invoker_base<R,Arg> {
    T t_;
public:
    function_object_invoker(T t):t_(t) {}

    R operator()(Arg arg) {
        return t_(arg);
    }
};

template <typename>
class function {
};

template <typename R, typename Arg> 
class function<R(Arg)> {
    invoker_base<R,Arg>* invoker_;
public:
    function(R (*func)(Arg)) : 
        invoker_(new function_ptr_invoker<R,Arg>(func)) {}

    template <typename C, typename MT> function(MT C::* func) : 
        invoker_(new member_ptr_invoker<R,Arg,C,MT>(func)) {}

    template <typename T> function(T t) : 
        invoker_(new function_object_invoker<R,Arg,T>(t)) {}

    R operator()(Arg arg) {
        return (*invoker_)(arg);
    }

    ~function() {
        delete invoker_;
    }
};

}   // namespace Hx
