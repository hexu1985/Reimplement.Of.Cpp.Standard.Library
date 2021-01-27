#ifndef MINI_STL_UNIQUE_PTR_INC
#define MINI_STL_UNIQUE_PTR_INC

#include <iostream>
#include <type_traits>
#include <algorithm>
#include <functional>

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

template <typename T, typename Deleter=default_delete<T>>
class unique_ptr {
public:
    using pointer = T *;
    using element_type = T;
    using deleter_type = Deleter;

private:
    element_type *px_;
    Deleter del_;

    template <typename U, typename E>
    friend class unique_ptr;

public:
    unique_ptr() noexcept: px_(nullptr), del_() {}

    unique_ptr(std::nullptr_t) noexcept: px_(nullptr), del_() {}

    unique_ptr(T *p) noexcept: px_(p), del_() {}

#if 0
    unique_ptr(T *p, Deleter &del): px_(p), del_(del) {}

    unique_ptr(T *p, Deleter &&del): px_(p), del_(std::move(del)) {}

    unique_ptr(unique_ptr &&u) noexcept: px_(u.px_), del_(std::move(u.del_))
    {
        u.px_ = nullptr;
    };

    template<typename U, typename E, typename = typename std::enable_if<std::is_convertible<U *, T *>::value>::type>
    unique_ptr(unique_ptr<U, E> &&u) noexcept: px_(u.px_), del_(std::move(u.del_))
    {
        u.px_ = nullptr;
    }
#else
    unique_ptr(pointer p,
        typename std::conditional<std::is_reference<Deleter>::value, Deleter, const Deleter &>::type del): 
        px_(p), del_(del) {}

    unique_ptr (pointer p, typename std::remove_reference<Deleter>::type &&del):
        px_(p), del_(std::move(del)) {}

    unique_ptr(unique_ptr &&u) noexcept: px_(u.px_), 
        del_(static_cast<typename std::conditional<std::is_reference<Deleter>::value, Deleter &, Deleter &&>::type>(u.del_))
    {
        u.px_ = nullptr;
    };

    template<typename U, typename E, typename = typename std::enable_if<std::is_convertible<U *, T *>::value>::type>
    unique_ptr(unique_ptr<U, E> &&u) noexcept: px_(u.px_), 
        del_(static_cast<typename std::conditional<std::is_reference<E>::value, E &, E &&>::type>(u.del_))
    {
        u.px_ = nullptr;
    }
#endif

    ~unique_ptr()
    {
        destroy();
    }

    unique_ptr &operator =(unique_ptr &&r) noexcept 
    {
        if (this == &r)
            return *this;

        destroy();
        std::swap(px_, r.px_);
        return *this;
    }

    T *release() noexcept 
    {
        T *p = px_;
        px_ = nullptr;
        return p;
    }

    void reset(T *p = nullptr) noexcept
    {
        destroy();
        px_ = p;
    }

    void swap(unique_ptr &other) noexcept 
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

    deleter_type &get_deleter() noexcept
    {
        return del_;
    }

    const deleter_type &get_deleter() const noexcept
    {
        return del_;
    }

private:
    void destroy() 
    {
        if (px_) {
            del_(px_);
            px_ = nullptr;
        }
    }
};

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args &&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename charT, typename traits, typename T, typename Deleter>
std::basic_ostream<charT, traits> &operator <<(
        std::basic_ostream<charT, traits> &os, 
        const unique_ptr<T, Deleter> &ptr)
{
    os << ptr.get();
    return os;
}

// comparaison operators
template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator ==(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() == rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator !=(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() != rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator <=(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() <= rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator <(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() < rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator >=(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() >= rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator>(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) noexcept
{
    return (lhs.get() > rhs.get());
}

template <typename T, typename D> 
inline bool operator ==(const unique_ptr<T, D> &lhs, std::nullptr_t) noexcept
{
    return !lhs;
}

template <typename T, typename D> 
inline bool operator ==(std::nullptr_t, const unique_ptr<T, D> &rhs) noexcept
{
    return !rhs;
}

template <typename T, typename D> 
inline bool operator !=(const unique_ptr<T, D> &lhs, std::nullptr_t) noexcept
{
    return (bool) lhs;
}

template <typename T, typename D> 
inline bool operator !=(std::nullptr_t, const unique_ptr<T, D> &rhs) noexcept
{
    return (bool) rhs;
}


}   // namespace mini_stl

namespace std {

template <typename T, typename Deleter>
struct hash<mini_stl::unique_ptr<T, Deleter>> {
    size_t operator ()(const mini_stl::unique_ptr<T, Deleter> &p)
    {
        return std::hash<typename mini_stl::unique_ptr<T, Deleter>::pointer>()(p.get());
    }
};

} // namespace std

#endif
