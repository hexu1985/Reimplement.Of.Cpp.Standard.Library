#if 0
template <typename T>
struct default_delete<T[]> {
    default_delete() = default;
    ~default_delete() = default;

    void operator()(T* p) const
    {
        delete [] p;
    }
};
#else
template <typename T>
struct default_delete<T[]> {
    default_delete() = default;
    ~default_delete() = default;

    default_delete(const default_delete& d) {}
    default_delete(default_delete&& d) {}

    template <typename U, 
             typename = typename std::enable_if<std::is_convertible<U, T>::value>::type>
    default_delete(const default_delete<U>& d) {}

    default_delete& operator=(const default_delete& d) { return *this; }
    default_delete& operator=(default_delete&& d) { return *this; }

    void operator()(T* p) const
    {
        delete [] p;
    }
};
#endif

template<typename T, typename Deleter>
class unique_ptr<T[], Deleter> {
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
        typename std::conditional<std::is_reference<Deleter>::value, Deleter, const Deleter&>::type del): 
        ptr_(p), del_(del) {}

    unique_ptr(pointer p, typename std::remove_reference<Deleter>::type&& del):
        ptr_(p), del_(std::move(del)) {}
#endif

    unique_ptr(unique_ptr&& u) noexcept: ptr_(u.ptr_), del_(std::forward<Deleter>(u.del_))
    {
        u.ptr_ = nullptr;
    };

    template<typename U, typename E, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
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

    unique_ptr& operator=(unique_ptr&& r) noexcept 
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

    T& operator[](size_t i) const
    {
        return ptr_[i];
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

// std::is_unbounded_array can use in C++20
template<class T>
struct is_unbounded_array: std::false_type {};
 
template<class T>
struct is_unbounded_array<T[]> : std::true_type {};

template <typename T, 
    typename = typename std::enable_if<is_unbounded_array<T>::value>::type
    >
unique_ptr<T> make_unique(size_t size)
{
    return unique_ptr<T>(new typename std::remove_extent<T>::type[size]);
}

