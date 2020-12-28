#ifndef MINI_STL_UNIQUE_PTR_INC
#define MINI_STL_UNIQUE_PTR_INC

#include <type_traits>

namespace mini_stl {

template <typename T>
struct default_delete {
    default_delete() = default;
    ~default_delete() = default;

    void operator ()(T *p) const
    {
        delete p;
    }
};

template<typename T, typename Deleter=default_delete<T>>
class unique_ptr
{
public:
    using pointer = T *;
    using element_type = T;
    using deleter_type = Deleter;

private:
    element_type *px_ = nullptr;
    Deleter del_;

public:
    unique_ptr() noexcept {}

    unique_ptr(std::nullptr_t) noexcept {}

    unique_ptr(T *p) noexcept: px_(p) {}

    unique_ptr(T *p, const Deleter &del): px_(p), del_(del) {}

    unique_ptr(T *p, Deleter &&del): px_(p), del_(std::move(del)) {}

    unique_ptr(unique_ptr<T> &&u) noexcept {
      std::swap(px_, u.px_);
      std::swap(del_, u.del_);
    };

    template<typename U, typename = typename std::enable_if<std::is_convertible<U *, T *>::value>::type>
    unique_ptr(unique_ptr<U> &&u) noexcept 
    {
        std::swap(px_, u.px_);
        std::swap(del_, u.del_);
    }

    ~unique_ptr()
    {
        destroy();
    }

    unique_ptr &operator =(unique_ptr<T> &&r) noexcept 
    {
        if (this == &r)
            return *this;

        destroy();
        std::swap(px_, r.px_);
        std::swap(del_, r.del_);
    }


    T *release() noexcept 
    {
        T *p = px_;
        px_ = nullptr;
        return p;
    }

    void reset(T *p) noexcept
    {
        unique_ptr<T> other(p);
        swap(other);
    }

    void swap(unique_ptr<T> &other) noexcept 
    {
        std::swap(px_, other.px_);
        std::swap(del_, other.del_);
    }

    T *get() const noexcept 
    {
        return px_;
    }

    T &operator *() const 
    {
        return *px_;
    }

    T *operator ->() const noexcept
    {
      return px_;
    }

    explicit operator bool() const noexcept 
    {
        return px_ != nullptr;
    }

    template<typename U, typename... Args>
    friend unique_ptr<U> make_unique(Args &&... args);

private:
    void destroy() 
    {
        if (px_) {
            del_(px_);
            px_ = nullptr;
        }
    }
};


// comparaison operators
template <typename T, typename U> 
inline bool operator ==(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() == r.get());
}

template <typename T, typename U> 
inline bool operator !=(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() != r.get());
}

template <typename T, typename U> 
inline bool operator <=(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() <= r.get());
}

template <typename T, typename U> 
inline bool operator <(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() < r.get());
}

template <typename T, typename U> 
inline bool operator >=(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() >= r.get());
}

template <typename T, typename U> 
inline bool operator>(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept
{
    return (l.get() > r.get());
}

}   // namespace mini_stl

#endif
