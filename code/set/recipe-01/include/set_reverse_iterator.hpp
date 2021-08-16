/**
 * A set::reverse_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_reverse_iterator {
    typedef binary_search::tree_node_t link_type;
    link_type* link = nullptr;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_reverse_iterator<T> this_type;
    typedef set_node<T> node_type;

    set_reverse_iterator() = default;

    explicit set_reverse_iterator(link_type* link_): link(link_) {}

    reference operator* () const
    {
        assert(link != nullptr);
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator-> () const
    {
        assert(link != nullptr);
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++ ()
    {
        next();
        return *this;
    }

    this_type operator++ (int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type& operator-- ()
    {
        prev();
        return *this;
    }

    this_type operator-- (int)
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
        assert(link != nullptr);
        link = tree_predecessor(link);
    }

    void prev()
    {
        assert(link != nullptr);
        link = tree_successor(link);
    }
};

/**
 * A set::const_reverse_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_const_reverse_iterator {
    typedef binary_search::tree_node_t link_type;
    link_type* link = nullptr;

    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_const_reverse_iterator<T> this_type;
    typedef set_node<T> node_type;
    typedef set_reverse_iterator<T> reverse_iterator;

    set_const_reverse_iterator() = default;

    explicit set_const_reverse_iterator(link_type* link_): link(link_) {}

    set_const_reverse_iterator(const reverse_iterator& iter): link(iter.link) {}

    reference operator* () const
    {
        assert(link != nullptr);
        return *static_cast<node_type*>(link)->valptr();
    }

    pointer operator-> () const
    {
        assert(link != nullptr);
        return static_cast<node_type*>(link)->valptr();
    }

    this_type& operator++ ()
    {
        next();
        return *this;
    }

    this_type operator++ (int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type& operator--()
    {
        prev();
        return *this;
    }

    this_type operator-- (int)
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
        assert(link != nullptr);
        link = tree_predecessor(link);
    }

    void prev()
    {
        assert(link != nullptr);
        link = tree_successor(link);
    }
};

/**
 * set reverse_iterator equality comparison.
 */
template <typename T>
inline
bool operator== (const set_reverse_iterator<T>& x, const set_const_reverse_iterator<T>& y)
{
    return (x.link == y.link);
}

/**
 * set reverse_iterator inequality comparison.
 */
template <typename T>
inline
bool operator!= (const set_reverse_iterator<T>& x, const set_const_reverse_iterator<T>& y)
{
    return !(x == y);
}
