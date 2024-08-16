/**
 * A forward_list::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct forward_list_iterator {
    typedef singly_linked::list_node_t link_type;
    link_type* link;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef forward_list_iterator<T> this_type;
    typedef forward_list_node<T> node_type;

    forward_list_iterator(): link(nullptr) {}

    explicit forward_list_iterator(link_type* link_): link(link_) {}

    reference operator*() const
    {
        assert(link != nullptr);
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator->() const
    {
        assert(link != nullptr);
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++()
    {
        next();
        return *this;
    }

    this_type operator++(int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    bool operator==(const this_type& other) const
    {
        return (this->link == other.link);
    }

    bool operator!=(const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr);
        link = link->next;
    }
};

/**
 * A forward_list::const_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct forward_list_const_iterator {
    typedef const singly_linked::list_node_t link_type;
    link_type* link;

    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef forward_list_const_iterator<T> this_type;
    typedef const forward_list_node<T> node_type;
    typedef forward_list_iterator<T> iterator;

    forward_list_const_iterator(): link(nullptr) {}

    explicit forward_list_const_iterator(link_type* link_): link(link_) {}

    forward_list_const_iterator(const iterator& iter): link(iter.link) {}

    reference operator*() const
    {
        assert(link != nullptr);
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator->() const
    {
        assert(link != nullptr);
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++()
    {
        next();
        return *this;
    }

    this_type operator++(int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    bool operator==(const this_type& other) const
    {
        return (this->link == other.link);
    }

    bool operator!=(const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr);
        link = link->next;
    }
};

/**
 * Forward list iterator equality comparison.
 */
template <typename T>
inline 
bool operator==(const forward_list_iterator<T>& x,
    const forward_list_const_iterator<T>& y)
{
    return (x.link == y.link);
}

/**
 * Forward list iterator inequality comparison.
 */
template <typename T>
inline 
bool operator!=(const forward_list_iterator<T>& x,
    const forward_list_const_iterator<T>& y)
{
    return !(x == y);
}
