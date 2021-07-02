// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_SET_INC
#define MINI_STL_SET_INC

#include "set_impl.h"

#include <memory>
#include <initializer_list>

namespace mini_stl {

/**
 * A helper node class for set.
 * This is just a binary search tree node with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct set_node: public binary_search::tree_node_t {
    // raw storage buffer for type T
    typename 
    std::aligned_storage<sizeof (T), std::alignment_of<T>::value>::type storage;

    T *valptr() noexcept
    {
        return static_cast<T *>(static_cast<void *>(&storage));
    }

    const T *valptr() const noexcept
    {
        return static_cast<const T *>(static_cast<const void *>(&storage));
    }
};

/**
 * A set::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_iterator {
    typedef binary_search::tree_node_t link_type;
    link_type *link = nullptr;

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_iterator<T> this_type;
    typedef set_node<T> node_type;

    set_iterator() = default;

    explicit set_iterator(link_type *link_): link(link_) {}

    reference operator *() const
    {
        assert(link != nullptr);
        return *static_cast<node_type *>(link)->valptr();
    }

    pointer operator ->() const
    {
        assert(link != nullptr);
        return static_cast<node_type *>(link)->valptr();
    }

    this_type &operator ++()
    {
        next();
        return *this;
    }

    this_type operator ++(int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type &operator --()
    {
        prev();
        return *this;
    }

    this_type operator --(int)
    {
        this_type tmp(*this);
        prev();
        return tmp;
    }

    bool operator ==(const this_type &other) const
    {
        return (this->link == other.link);
    }

    bool operator !=(const this_type &other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr);
        link = tree_successor(link);
    }

    void prev()
    {
        assert(link != nullptr);
        link = tree_predecessor(link);
    }
};

/**
 * A set::const_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct set_const_iterator {
    typedef binary_search::tree_node_t link_type;
    link_type *link = nullptr;

    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef set_const_iterator<T> this_type;
    typedef set_node<T> node_type;
    typedef set_iterator<T> iterator;

    set_const_iterator() = default;

    explicit set_const_iterator(link_type *link_): link(link_) {}

    set_const_iterator(const iterator &iter): link(iter.link) {}

    reference operator *() const
    {
        assert(link != nullptr);
        return *static_cast<node_type *>(link)->valptr();
    }

    pointer operator ->() const
    {
        assert(link != nullptr);
        return static_cast<node_type *>(link)->valptr();
    }

    this_type &operator ++()
    {
        next();
        return *this;
    }

    this_type operator ++(int)
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type &operator --()
    {
        prev();
        return *this;
    }

    this_type operator --(int)
    {
        this_type tmp(*this);
        prev();
        return tmp;
    }

    bool operator ==(const this_type &other) const
    {
        return (this->link == other.link);
    }

    bool operator !=(const this_type &other) const
    {
        return !(*this == other);
    }

    void next()
    {
        assert(link != nullptr);
        link = tree_successor(link);
    }

    void prev()
    {
        assert(link != nullptr);
        link = tree_predecessor(link);
    }
};

/**
 * set iterator equality comparison.
 */
template <typename T>
inline
bool operator ==(const set_iterator<T> &x, const set_const_iterator<T> &y)
{
    return (x.link == y.link);
}

/**
 * set iterator inequality comparison.
 */
template <typename T>
inline
bool operator !=(const set_iterator<T> &x, const set_const_iterator<T> &y)
{
    return !(x == y);
}

/**
 * Sets are containers that store unique elements following a specific order.
 */
template <typename T, typename Compare = std::less<T>,
          typename Alloc = std::allocator<T>>
class set {
    typedef binary_search::tree_t tree_type;
    typedef binary_search::tree_node_t link_type;
    typedef typename Alloc::template rebind<set_node<T>>::other node_alloc_type;

    Compare less_;
    node_alloc_type node_alloc_;
    tree_type tree_;

public:
    typedef T key_type;
    typedef T value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Alloc allocator_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename std::allocator_traits<allocator_type>::pointer pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;
    typedef set_iterator<const value_type> iterator;
    typedef set_const_iterator<const value_type> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef set_node<value_type> node_type;

    /**
     * empty container constructors (default constructor)
     * Constructs an empty container, with no elements.
     */
    set(): set(key_compare(), allocator_type()) {}

    explicit set(const key_compare &comp,
                 const allocator_type &alloc = allocator_type()):
        less_(comp), node_alloc_(alloc)
    {
        initialize();
    }

    explicit set(const allocator_type &alloc): set(key_compare(), alloc) {}

    /**
     *  range constructor
     *  Constructs a container with as many elements as the range [first,last), 
     *  with each element emplace-constructed from its corresponding element in that range.
     */
    template <typename InputIterator>
    set(InputIterator first, InputIterator last,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type()): set(comp, alloc)
    {
        try
        {
            insert(first, last);
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    template <typename InputIterator>
    set(InputIterator first, InputIterator last, 
        const allocator_type &alloc): set(first, last, key_compare(), alloc)
    {}

    set(std::initializer_list<value_type> il,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type()): set(il.begin(), il.end(), comp, alloc)
    {
    }

    /**
     * Set destructor
     * Destroys the container object.
     */
    ~set()
    {
        finalize();
    }

    /**
     * Return iterator to beginning
     * Returns an iterator referring to the first element in the set container.
     */
    iterator begin() noexcept
    {
        link_type *x = empty() ? nullptr : tree_minimum(tree_.root);

        return iterator(x);
    }

    const_iterator begin() const noexcept
    {
        link_type *x = empty() ? nullptr : tree_minimum(tree_.root);

        return const_iterator(x);
    }

    /**
     * Return iterator to end
     * Returns an iterator referring to the past-the-end element in the set container.
     */
    iterator end() noexcept
    {
        return iterator(nullptr);
    }

    const_iterator end() const noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * Return const_iterator to beginning
     * Returns a const_iterator pointing to the first element in the container.
     */
    const_iterator cbegin() const noexcept
    {
        link_type *x = empty() ? nullptr : tree_minimum(tree_.root);

        return const_iterator(x);
    }

    /**
     * Return const_iterator to end
     * Returns a const_iterator pointing to the past-the-end element in the container.
     */
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * Test whether container is empty
     * Returns whether the set container is empty (i.e. whether its size is 0).
     */
    bool empty() const noexcept
    {
        return tree_is_empty(&tree_);
    }

    std::pair<iterator, bool> insert(const value_type &val)
    {
        std::pair<link_type *, bool> ret = insert(tree_.root, val);
        return std::make_pair(iterator(ret.first), ret.second);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        while (first != last) {
            insert(*first++);
        }
    }

private:
    node_type *get_node()
    {
        return node_alloc_.allocate(1);
    }

    void put_node(node_type *node)
    {
        node_alloc_.deallocate(node, 1);
    }

    template <typename ...Args>
    link_type *create_node(Args &&...args)
    {
        node_type *node = get_node();
        tree_node_init(node);
        try
        {
            new (node->valptr()) T(std::forward<Args>(args)...);
        }
        catch (...)
        {
            put_node(node);
            throw;
        }
        return static_cast<link_type *>(node);
    }

    void destroy_node(link_type *link)
    {
        node_type *node = static_cast<node_type *>(link);
        node->valptr()->~T();
        node_alloc_.deallocate(node, 1);
    }

    void initialize()
    {
        tree_init(&tree_);
    }

    void finalize()
    {
        destroy_tree(tree_.root);
    }

    void destroy_tree(link_type *root)
    {
        if (root == NULL)
            return;

        destroy_tree(root->left);
        destroy_tree(root->right);
        destroy_node(root);
    }

    std::pair<link_type *, bool> insert(link_type *root, const value_type &val)
    {
        link_type *y = NULL;
        link_type *x = root;
        while (x != NULL) {
            y = x;
            const value_type &x_val = *static_cast<node_type *>(x)->valptr();
            if (less_(val, x_val)) {        // val < x->val
                x = x->left;
            } else if (less_(x_val, val)) { // x->val < val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, false);
            }
        }

        link_type *z = create_node(val);
        if (y == NULL) {
            tree_set_root(&tree_, z);
        } else if (less_(val, *static_cast<node_type *>(y)->valptr())) {
            tree_set_left_child(y, z);
        } else {
            tree_set_right_child(y, z);
        }
        return std::make_pair(z, true);
    }

};


} // namespace mini_stl

#endif // MINI_STL_SET_INC
