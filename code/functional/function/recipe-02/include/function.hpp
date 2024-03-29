#pragma once

#include <utility>  // for std::forward
#include <type_traits>

namespace Hx {

template <typename R, typename... Args> 
class invoker_base {
public:
    virtual R operator()(Args... args) const=0;
    virtual ~invoker_base() {}
    virtual invoker_base* clone() const=0;
};

template <typename R, typename... Args> 
class function_ptr_invoker : public invoker_base<R,Args...> {
    R (*func_)(Args...);

public:
    function_ptr_invoker(R (*func)(Args...)):func_(func) {}

    R operator()(Args... args) const override {
        return (func_)(std::forward<Args>(args)...);
    }

    invoker_base<R,Args...>* clone() const override {
        return new function_ptr_invoker(func_);
    }
};

namespace detail {

template <typename R, typename MT, typename C, typename T1, typename... Args>
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

    R operator()(Args... args) const override {
        return detail::invoke_memptr<R>(memptr_, std::forward<Args>(args)...);
    }

    invoker_base<R,Args...>* clone() const override {
        return new member_ptr_invoker(memptr_);
    }
};

template <typename R, typename F, typename... Args> 
class function_object_invoker : public invoker_base<R,Args...> {
    mutable F f_;

public:
    function_object_invoker(F f):f_(f) {}

    R operator()(Args... args) const override {
        return f_(std::forward<Args>(args)...);
    }

    invoker_base<R,Args...>* clone() const override {
        return new function_object_invoker(f_);
    }
};

template <typename>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
    invoker_base<R,Args...>* invoker_ = nullptr;

public:
    using ResultType = R;

    // constructs
    function() noexcept {}

    function (nullptr_t fn) noexcept {}

    function(R (*func)(Args...)) : 
        invoker_(new function_ptr_invoker<R,Args...>(func)) {}

    template <typename MT, typename C> function(MT C::* mptr) :
        invoker_(new member_ptr_invoker<R,MT,C,Args...>(mptr)) {}

    template <typename F> function(F f) : 
        invoker_(new function_object_invoker<R,F,Args...>(f)) {}

    // copy construct
    function(const function& x) : invoker_(x.invoker_ ? x.invoker_->clone() : 0) {}

    // move construct
    function(function&& x) : invoker_(x.invoker_) {
        x.invoker_ = nullptr;
    }

    // assign operator
    function& operator=(nullptr_t fn) {
        delete invoker_;
        invoker_ = nullptr;
        return *this;
    }

    function& operator=(R (*func)(Args...)) {
        delete invoker_;
        invoker_ = new function_ptr_invoker<R,Args...>(func); 
        return *this;
    }

    template <typename MT, typename C> 
    function& operator=(MT C::* mptr) {
        delete invoker_;
        invoker_ = new member_ptr_invoker<R,MT,C,Args...>(mptr);
        return *this;
    }

    template <typename T>
    function& operator=(T t) {
        delete invoker_;
        invoker_ = new function_object_invoker<R,T,Args...>(t); 
        return *this;
    }

    function& operator=(const function& rhs) {
        function(rhs).swap(*this);
        return *this;
    }

    // move assignment
    function& operator=(function&& rhs) noexcept {
        rhs.swap(*this);
        function().swap(rhs);
        return *this;
    }

    // swap
    function& swap(function& rhs) noexcept {
        std::swap(invoker_, rhs.invoker_);
        return *this;
    }

    // callable operator
    R operator()(Args... args) const {
        return (*invoker_)(std::forward<Args>(args)...);
    }

    // Check if callable
    explicit operator bool() const noexcept {
        return invoker_;
    }

    // destroy function
    ~function() {
        delete invoker_;
    }
};

}   // namespace Hx
