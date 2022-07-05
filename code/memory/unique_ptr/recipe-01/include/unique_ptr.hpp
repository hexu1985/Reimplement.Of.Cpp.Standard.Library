#ifndef MINI_STL_UNIQUE_PTR_INC
#define MINI_STL_UNIQUE_PTR_INC

#include <iostream>
#include <type_traits>
#include <algorithm>
#include <functional>

namespace Hx {

#if 0
template <typename T>
struct default_delete {
    default_delete() = default;
    ~default_delete() = default;

    void operator() (T* p) const
    {
        delete p;
    }
};
#else
template <typename T>
struct default_delete {
    default_delete() = default;
    ~default_delete() = default;

    default_delete(const default_delete& d) {}
    default_delete(default_delete&& d) {}

    template <typename U, 
             typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
    default_delete(const default_delete<U>& d) {}

    default_delete& operator= (const default_delete& d) { return *this; }
    default_delete& operator= (default_delete&& d) { return *this; }

    void operator() (T* p) const
    {
        delete p;
    }
};
#endif

template <typename T, typename Deleter=default_delete<T>>
class unique_ptr {
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = Deleter;

private:
    element_type* ptr_;
    Deleter del_;

    template <typename U, typename E>
    friend class unique_ptr;

public:
    unique_ptr() noexcept: ptr_(nullptr), del_() {}

    unique_ptr(std::nullptr_t) noexcept: ptr_(nullptr), del_() {}

    unique_ptr(T* p) noexcept: ptr_(p), del_() {}

#if 0
    unique_ptr(T* p, Deleter& del): ptr_(p), del_(del) {}

    unique_ptr(T* p, Deleter&& del): ptr_(p), del_(std::move(del)) {}
#else
    unique_ptr(pointer p,
        typename std::conditional<std::is_reference<Deleter>::value, Deleter, const Deleter &>::type del): 
        ptr_(p), del_(del) {}

    unique_ptr(pointer p, typename std::remove_reference<Deleter>::type&& del):
        ptr_(p), del_(std::move(del)) {}
#endif

    unique_ptr(unique_ptr&& u) noexcept: ptr_(u.ptr_), del_(std::forward<Deleter>(u.del_))
    {
        u.ptr_ = nullptr;
    };

    template <typename U, typename E, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
    unique_ptr(unique_ptr<U, E>&& u) noexcept: ptr_(u.ptr_), del_(std::forward<E>(u.del_))
    {
        u.ptr_ = nullptr;
    }

    ~unique_ptr()
    {
        if (ptr_ != nullptr) {
            del_(ptr_);
        }
    }

    unique_ptr& operator= (unique_ptr&& r) noexcept 
    {
        if (this != &r) {
            reset(r.release());
            // forward deleter
            del_ = std::forward<deleter_type>(r.del_);
        }
        return *this;
    }

    T* release() noexcept 
    {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }

    void reset(T* p = nullptr) noexcept
    {
        T* old_ptr = ptr_;
        ptr_ = p;
        if (old_ptr != nullptr) {
            del_(old_ptr);
        }
    }

    void swap(unique_ptr& other) noexcept 
    {
        if (this != &other) {
            using std::swap;
            // forward is already inside of swap
            swap(ptr_, other.ptr_);
            swap(del_, other.del_);
        }
    }

    T* get() const noexcept 
    {
        return ptr_;
    }

    T& operator* () const 
    {
        return *ptr_;
    }

    T* operator-> () const noexcept
    {
        return ptr_;
    }

    explicit operator bool() const noexcept 
    {
        return ptr_ != nullptr;
    }

    deleter_type& get_deleter() noexcept
    {
        return del_;
    }

    const deleter_type& get_deleter() const noexcept
    {
        return del_;
    }
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename charT, typename traits, typename T, typename Deleter>
std::basic_ostream<charT, traits>& operator<< (
        std::basic_ostream<charT, traits>& os, const unique_ptr<T, Deleter>& ptr)
{
    os << ptr.get();
    return os;
}

// comparaison operators
template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator== (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() == rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator!= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() != rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator<= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() <= rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator< (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() < rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator>= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() >= rhs.get());
}

template <typename T1, typename D1, typename T2, typename D2> 
inline bool operator> (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs) noexcept
{
    return (lhs.get() > rhs.get());
}

template <typename T, typename D> 
inline bool operator== (const unique_ptr<T, D>& lhs, std::nullptr_t) noexcept
{
    return !lhs;
}

template <typename T, typename D> 
inline bool operator== (std::nullptr_t, const unique_ptr<T, D>& rhs) noexcept
{
    return !rhs;
}

template <typename T, typename D> 
inline bool operator!= (const unique_ptr<T, D>& lhs, std::nullptr_t) noexcept
{
    return (bool) lhs;
}

template <typename T, typename D> 
inline bool operator!= (std::nullptr_t, const unique_ptr<T, D>& rhs) noexcept
{
    return (bool) rhs;
}

}   // namespace Hx

namespace std {

template <typename T, typename Deleter>
struct hash<Hx::unique_ptr<T, Deleter>> {
    size_t operator() (const Hx::unique_ptr<T, Deleter>& p)
    {
        return std::hash<typename Hx::unique_ptr<T, Deleter>::pointer>()(p.get());
    }
};

} // namespace std

#endif
