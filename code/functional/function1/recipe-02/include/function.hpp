#pragma once

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

template <typename R, typename MT, typename C, typename Arg> 
class member_ptr_invoker : public invoker_base<R,Arg> {
    MT C::* memptr_;

public:
    member_ptr_invoker(MT C::* memptr): memptr_(memptr) {}

    R operator()(Arg arg) {
        return (arg->*memptr_)();
    }
};

template <typename R, typename F, typename Arg> 
class function_object_invoker : public invoker_base<R,Arg> {
    F f_;
public:
    function_object_invoker(F f):f_(f) {}

    R operator()(Arg arg) {
        return f_(arg);
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

    template <typename MT, typename C> function(MT C::* func) : 
        invoker_(new member_ptr_invoker<R,MT,C,Arg>(func)) {}

    template <typename F> function(F f) : 
        invoker_(new function_object_invoker<R,F,Arg>(f)) {}

    R operator()(Arg arg) {
        return (*invoker_)(arg);
    }

    ~function() {
        delete invoker_;
    }
};

}   // namespace Hx
