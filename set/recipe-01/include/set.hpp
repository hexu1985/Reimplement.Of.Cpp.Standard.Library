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
        const allocator_type &alloc = allocator_type()): less_(comp), node_alloc_(alloc)
    {
        initialize();
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
        const allocator_type &alloc): set(first, last, key_compare(), alloc) {}

    /**
     * initializer list constructor
     * Constructs a container with a copy of each of the elements in il.
     */
    set(std::initializer_list<value_type> il,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type()): set(il.begin(), il.end(), comp, alloc) {}

    set(std::initializer_list<value_type> il, const allocator_type& alloc): set(il, key_compare(), alloc) {}

    /**
     * copy constructor (and copying with allocator)
     * Constructs a container with a copy of each of the elements in x.
     */
    set(const set &x): set(x, x.get_allocator()) {}

    set(const set &x, const allocator_type &alloc): 
        less_(x.less_), node_alloc_(alloc)
    {
        initialize();
        tree_.root = clone_tree(x.tree_.root);
    }

    /**
     * move constructor (and moving with allocator)
     * Constructs a container that acquires the elements of x.
     * If alloc is specified and is different from x's allocator, the elements are moved. 
     * Otherwise, no elements are constructed (their ownership is directly transferred).
     * x is left in an unspecified but valid state.
     */
    set(set &&x): less_(std::move(x.less_)), node_alloc_(std::move(x.node_alloc_))
    {
        initialize();
        tree_.root = x.tree_.root;
        x.initialize();
    }

    set(set&& x, const allocator_type& alloc): less_(std::move(x.less_)), node_alloc_(alloc)
    {
        initialize();
        try
        {
            tree_.root = move_tree(x.tree_.root);
            x.clear();
        }
        catch (...)
        {
            x.clear();
            throw;
        }
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

    /**
     * Return container size
     * Returns the number of elements in the set container.
     */
    size_type size() const noexcept
    {
        return tree_size(tree_.root);
    }

    /**
     * Insert element
     * Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.
     */
    std::pair<iterator, bool> insert(const value_type &val)
    {
        std::pair<link_type *, bool> ret = tree_insert(tree_.root, val);
        return std::make_pair(iterator(ret.first), ret.second);
    }

    std::pair<iterator,bool> insert (value_type&& val)
    {
        std::pair<link_type *, bool> ret = tree_insert(tree_.root, std::move(val));
        return std::make_pair(iterator(ret.first), ret.second);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        while (first != last) {
            insert(*first++);
        }
    }

    /**
     * Clear content
     * Removes all elements from the set container (which are destroyed), leaving the container with a size of 0.
     */
    void clear() noexcept
    {
        finalize();
        initialize();
    }

    /**
     * Construct and insert element
     * Inserts a new element in the set, if unique. This new element is constructed in place using args as the arguments for its construction.
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        std::pair<link_type *, bool> ret = tree_emplace(tree_.root, std::forward<Args>(args)...);
        return std::make_pair(iterator(ret.first), ret.second);
    }

    /**
     * Get iterator to element
     * Searches the container for an element equivalent to val and returns an iterator to it if found, 
     * otherwise it returns an iterator to set::end.
     */
    const_iterator find(const value_type &val) const
    {
        return const_iterator(tree_find(tree_.root, val));
    }

    iterator find(const value_type &val)
    {
        return iterator(tree_find(tree_.root, val));
    }

    /**
     * Count elements with a specific value
     * Searches the container for elements equivalent to val and returns the number of matches.
     */
    size_type count(const value_type &val) const
    {
        return tree_find(tree_.root, val) == NULL ? 0 : 1;
    }

    /**
     * Get allocator
     * Returns a copy of the allocator object associated with the set.
     */
    allocator_type get_allocator() const noexcept
    {
        return allocator_type(node_alloc_);
    }

private:
    const value_type &get_value(link_type *node)
    {
        return *static_cast<node_type *>(node)->valptr();
    }

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

    std::pair<link_type *, bool> tree_insert(link_type *root, const value_type &val)
    {
        link_type *y = NULL;
        link_type *x = root;
        while (x != NULL) {
            y = x;
            const value_type &x_val = get_value(x);
            if (less_(val, x_val)) {        // val < x->val
                x = x->left;
            } else if (less_(x_val, val)) { // x->val < val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, false);
            }
        }

        link_type *z = create_node(val);
        tree_set_child(y, z);
        return std::make_pair(z, true);
    }

    std::pair<link_type *, bool> tree_insert(link_type *root, value_type &&val)
    {
        link_type *y = NULL;
        link_type *x = root;
        while (x != NULL) {
            y = x;
            const value_type &x_val = get_value(x);
            if (less_(val, x_val)) {        // val < x->val
                x = x->left;
            } else if (less_(x_val, val)) { // x->val < val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, false);
            }
        }

        link_type *z = create_node(std::move(val));
        tree_set_child(y, z);
        return std::make_pair(z, true);
    }

    template <typename ... Args>
    std::pair<link_type *, bool> tree_emplace(link_type *root, Args &&... args)
    {
        value_type val = value_type(std::forward<Args>(args)...);
        link_type *y = NULL;
        link_type *x = root;
        while (x != NULL) {
            y = x;
            const value_type &x_val = get_value(x);
            if (less_(val, x_val)) {        // val < x->val
                x = x->left;
            } else if (less_(x_val, val)) { // x->val < val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, false);
            }
        }

        link_type *z = create_node(std::move(val));
        tree_set_child(y, z);
        return std::make_pair(z, true);
    }

    void tree_set_child(link_type *p, link_type *c)
    {
        if (p == NULL) {
            tree_set_root(&tree_, c);
        } else if (less_(get_value(c), get_value(p))) {
            tree_set_left_child(p, c);
        } else {
            tree_set_right_child(p, c);
        }
    }

    link_type *tree_find(link_type *root, const value_type &val) const {
        link_type *x = root;
        while (x != NULL) {
            const value_type &x_val = *static_cast<node_type *>(x)->valptr();
            if (less_(val, x_val)) {        // val < x->val
                x = x->left;
            } else if (less_(x_val, val)) { // x->val < val
                x = x->right;
            } else {    // val == x->val
                break;
            }
        }
        return x;
    }

    link_type *clone_tree(link_type *root) {
        if (root == NULL)
            return NULL;

        link_type *left = NULL;
        link_type *right = NULL;
        link_type *node = NULL;

        try 
        {
            // clone left child tree
            left = clone_tree(root->left);

            // clone right child tree
            right = clone_tree(root->right);

            // clone root node
            node = create_node(*static_cast<const node_type *>(root)->valptr());
            tree_set_left_child(node, left);
            tree_set_right_child(node, right);
            return node;
        }
        catch (...)
        {
            if (left) destroy_tree(left);
            if (right) destroy_tree(right);
            if (node) destroy_node(node);
            throw;
        }
    }

    link_type *move_tree(link_type *root) {
        if (root == NULL)
            return NULL;

        link_type *left = NULL;
        link_type *right = NULL;
        link_type *node = NULL;

        try 
        {
            // move left child tree
            left = move_tree(root->left);

            // move right child tree
            right = move_tree(root->right);

            // move root node
            node = create_node(std::move(*static_cast<const node_type *>(root)->valptr()));
            tree_set_left_child(node, left);
            tree_set_right_child(node, right);
            return node;
        }
        catch (...)
        {
            if (left) destroy_tree(left);
            if (right) destroy_tree(right);
            if (node) destroy_node(node);
            throw;
        }
    }
};


} // namespace mini_stl

#endif // MINI_STL_SET_INC
