#pragma once

#include <utility>  // for std::forward

namespace Hx {

template <typename R, typename... Args> 
class invoker_base {
public:
    virtual R operator()(Args... args)=0;
    virtual ~invoker_base() {}
    virtual invoker_base* clone() const=0;
};

template <typename R, typename... Args> 
class function_ptr_invoker : public invoker_base<R,Args...> {
    R (*func_)(Args...);

public:
    function_ptr_invoker(R (*func)(Args...)):func_(func) {}

    R operator()(Args... args) override 
    {
        return (func_)(std::forward<Args>(args)...);
    }

    invoker_base<R,Args...>* clone() const override 
    {
        return new function_ptr_invoker(func_);
    }
};

#if 0
template <typename R, typename Args, typename T> 
class member_ptr_invoker : public invoker_base<R,Args> {
    R (T::*func_)(Args);
    T* t_;

public:
    member_ptr_invoker(R (T::*func)(Args),T* t):func_(func),t_(t) {}

    R operator()(Args arg) {
        return (t_->*func_)(arg);
    }
};
#endif

template <typename T, typename R, typename... Args> 
class function_object_invoker : public invoker_base<R,Args...> {
    T t_;

public:
    function_object_invoker(T t):t_(t) {}

    R operator()(Args... args) override
    {
        return t_(std::forward<Args>(args)...);
    }

    invoker_base<R,Args...>* clone() const override 
    {
        return new function_object_invoker(t_);
    }
};

template <typename>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
    invoker_base<R,Args...>* invoker_ = nullptr;

public:
    using ResultType = R;

    function() {}

    function(R (*func)(Args...)) : 
        invoker_(new function_ptr_invoker<R,Args...>(func)) {}

#if 0
    template <typename T> function(R (T::*func)(Arg),T* p) : 
        invoker_(new member_ptr_invoker<R,Arg,T>(func,p)) {}
#endif

    template <typename T> function(T t) : 
        invoker_(new function_object_invoker<T,R,Args...>(t)) {}

    function(const function& x) : invoker_(x.invoker_ ? x.invoker_->clone() : 0)
    {}

    function(function&& x) : invoker_(x.invoker_)
    {
        x.invoker_ = nullptr;
    }

    /*
    R operator()(Args... args) {
        return (*invoker_)(std::forward<Args>(args)...);
    }
    */
    template <typename... CArgs>
    ResultType operator()(CArgs... args)
    {
        return (*invoker_)(std::forward<CArgs>(args)...);
    }


    ~function() {
        delete invoker_;
    }

};

}   // namespace Hx
