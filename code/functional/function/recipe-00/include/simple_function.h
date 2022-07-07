#ifndef SIMPLE_FUNCTION_INC
#define SIMPLE_FUNCTION_INC

template <typename R, typename Arg> class invoker_base {
public:
  virtual R operator()(Arg arg)=0;
  virtual ~invoker_base() {}
};

template <typename R, typename Arg> class function_ptr_invoker 
  : public invoker_base<R,Arg> {
  R (*func_)(Arg);
public:
  function_ptr_invoker(R (*func)(Arg)):func_(func) {}

  R operator()(Arg arg) {
    return (func_)(arg);
  }
};

template <typename R, typename Arg, typename T> 
class member_ptr_invoker : 
  public invoker_base<R,Arg> {
  R (T::*func_)(Arg);
  T* t_;
public:
  member_ptr_invoker(R (T::*func)(Arg),T* t)
    :func_(func),t_(t) {}

  R operator()(Arg arg) {
    return (t_->*func_)(arg);
  }
};

template <typename R, typename Arg, typename T> 
class function_object_invoker : 
  public invoker_base<R,Arg> {
  T t_;
public:
  function_object_invoker(T t):t_(t) {}

  R operator()(Arg arg) {
    return t_(arg);
  }
};

template <typename R, typename Arg> class function1 {
  invoker_base<R,Arg>* invoker_;
public:
  function1(R (*func)(Arg)) : 
  invoker_(new function_ptr_invoker<R,Arg>(func)) {}
 
  template <typename T> function1(R (T::*func)(Arg),T* p) : 
    invoker_(new member_ptr_invoker<R,Arg,T>(func,p)) {}

  template <typename T> function1(T t) : 
    invoker_(new function_object_invoker<R,Arg,T>(t)) {}

  R operator()(Arg arg) {
    return (*invoker_)(arg);
  }

  ~function1() {
    delete invoker_;
  }
};

#endif
