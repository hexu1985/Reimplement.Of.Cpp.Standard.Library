/**
 * A list::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct list_iterator {
    typedef doubly_linked::list_node_t link_type;
    link_type* link = nullptr;

    typedef list_iterator<T> this_type;
    typedef list_node<T> node_type;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    list_iterator() = default;

    explicit list_iterator(link_type* link_): link(link_) {}

    reference operator* () const 
    {
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator-> () const 
    {
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++ ()    // prefix increment
    {
        next();
        return *this;
    }

    this_type operator++ (int)    // postfix increment
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type& operator-- ()    // prefix decrement
    {
        prev();
        return *this;
    }

    this_type operator-- (int)    // postfix decrement
    {
        this_type tmp(*this);
        prev();
        return tmp;
    }

    bool operator== (const this_type& other) const
    {
        return (this->link == other.link);
    }

    bool operator!= (const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        link = link->next;
    }

    void prev()
    {
        link = link->prev;
    }
};

template <typename T>
struct list_const_iterator {
    typedef const doubly_linked::list_node_t link_type;
    link_type* link = nullptr;

    typedef list_const_iterator<T> this_type;
    typedef const list_node<T> node_type;
    typedef list_iterator<T> iterator;

    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    list_const_iterator() = default;

    explicit list_const_iterator(link_type* link_): link(link_) {}

    list_const_iterator(const iterator& iter): link(iter.link) {}

    reference operator* () const 
    {
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator-> () const 
    { 
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++ ()    // prefix increment
    {
        next();
        return *this;
    }

    this_type operator++ (int)    // postfix increment
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type& operator-- ()    // prefix decrement
    {
        prev();
        return *this;
    }

    this_type operator-- (int)    // postfix decrement
    {
        this_type tmp(*this);
        prev();
        return tmp;
    }

    bool operator== (const this_type& other) const
    {
        return (this->link == other.link);
    }

    bool operator!= (const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        link = link->next;
    }

    void prev()
    {
        link = link->prev;
    }
};

/**
 * List iterator equality comparison.
 */
template <typename T>
inline 
bool operator== (const list_iterator<T>& x, const list_const_iterator<T>& y)
{
    return (x.link == y.link);
}

/**
 * List iterator inequality comparison.
 */
template <typename T>
inline 
bool operator!= (const list_iterator<T>& x, const list_const_iterator<T>& y)
{
    return !(x == y);
}


