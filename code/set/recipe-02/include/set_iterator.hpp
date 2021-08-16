/**
 * A set::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_iterator {
    typedef red_black::tree_t tree_type;
    typedef red_black::tree_node_t link_type;
    tree_type* tree = nullptr;
    link_type* link = nullptr;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_iterator<T> this_type;
    typedef set_node<T> node_type;

    set_iterator() = default;

    set_iterator(tree_type* tree_, link_type* link_): tree(tree_), link(link_) {}

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
        return (this->tree == other.tree && this->link == other.link);
    }

    bool operator!= (const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr && link != nullptr);
        if (link != &tree->nil) 
            link = tree_successor(tree, link);
        else
            link = tree_minimum(tree, tree->root);
    }

    void prev()
    {
        assert(link != nullptr && link != nullptr);
        if (link != &tree->nil)
            link = tree_predecessor(tree, link);
        else
            link = tree_maximum(tree, tree->root);
    }
};

/**
 * A set::const_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_const_iterator {
    typedef red_black::tree_t tree_type;
    typedef red_black::tree_node_t link_type;
    tree_type* tree = nullptr;
    link_type* link = nullptr;

    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_const_iterator<T> this_type;
    typedef set_node<T> node_type;
    typedef set_iterator<T> iterator;

    set_const_iterator() = default;

    set_const_iterator(tree_type* tree_, link_type* link_): tree(tree_), link(link_) {}

    set_const_iterator(const iterator& iter): tree(iter.tree), link(iter.link) {}

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
        return (this->tree == other.tree && this->link == other.link);
    }

    bool operator!= (const this_type& other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr && link != nullptr);
        if (link != &tree->nil) 
            link = tree_successor(tree, link);
        else
            link = tree_minimum(tree, tree->root);
    }

    void prev()
    {
        assert(link != nullptr && link != nullptr);
        if (link != &tree->nil)
            link = tree_predecessor(tree, link);
        else
            link = tree_maximum(tree, tree->root);
    }
};

/**
 * set iterator equality comparison.
 */
template <typename T>
inline
bool operator== (const set_iterator<T>& x, const set_const_iterator<T>& y)
{
    return (x.tree == y.tree && x.link == y.link);
}

/**
 * set iterator inequality comparison.
 */
template <typename T>
inline
bool operator!= (const set_iterator<T>& x, const set_const_iterator<T>& y)
{
    return !(x == y);
}

