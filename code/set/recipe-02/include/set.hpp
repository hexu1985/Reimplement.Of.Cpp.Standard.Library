// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_SET_INC
#define MINI_STL_SET_INC

#include "red_black_tree.hpp"

#include <memory>
#include <limits>
#include <initializer_list>

namespace Hx {

/**
 * A helper node class for set.
 * This is just a binary search tree node with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct set_node: public red_black::tree_node_t {
    // raw storage buffer for type T
    // typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;  // deprecated in C++23
    struct alignas(alignof(T)) { char data[sizeof(T)]; } storage;

    T* valptr() noexcept
    {
        return static_cast<T*>(static_cast<void*>(&storage));
    }

    const T* valptr() const noexcept
    {
        return static_cast<const T*>(static_cast<const void*>(&storage));
    }
};

#include "set_iterator.hpp"
#include "set_reverse_iterator.hpp"

/**
 * Sets are containers that store unique elements following a specific order.
 */
template <typename T, typename Compare = std::less<T>,
          typename Alloc = std::allocator<T>>
class set {
    typedef red_black::tree_t tree_type;
    typedef red_black::tree_node_t link_type;
    typedef typename Alloc::template rebind<set_node<T>>::other node_alloc_type;

    Compare less_;
    node_alloc_type node_alloc_;
    tree_type* tree_ = nullptr;

public:
    typedef T key_type;
    typedef T value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Alloc allocator_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename std::allocator_traits<allocator_type>::pointer pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;
    typedef set_iterator<const value_type> iterator;
    typedef set_const_iterator<const value_type> const_iterator;
    typedef set_reverse_iterator<const value_type> reverse_iterator;
    typedef set_const_reverse_iterator<const value_type> const_reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef set_node<value_type> node_type;

    /**
     * empty container constructors (default constructor)
     * Constructs an empty container, with no elements.
     */
    set(): set(key_compare(), allocator_type()) {}

    explicit set(const key_compare& comp,
                 const allocator_type& alloc = allocator_type()):
        less_(comp), node_alloc_(alloc)
    {
        initialize();
    }

    explicit set(const allocator_type& alloc): set(key_compare(), alloc) {}

    /**
     *  range constructor
     *  Constructs a container with as many elements as the range [first,last), 
     *  with each element emplace-constructed from its corresponding element in that range.
     */
    template <typename InputIterator>
    set(InputIterator first, InputIterator last,
        const key_compare& comp = key_compare(),
        const allocator_type& alloc = allocator_type()): less_(comp), node_alloc_(alloc)
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
        const allocator_type& alloc): set(first, last, key_compare(), alloc) {}

    /**
     * initializer list constructor
     * Constructs a container with a copy of each of the elements in il.
     */
    set(std::initializer_list<value_type> il,
        const key_compare& comp = key_compare(),
        const allocator_type& alloc = allocator_type()): set(il.begin(), il.end(), comp, alloc) {}

    set(std::initializer_list<value_type> il, const allocator_type& alloc): set(il, key_compare(), alloc) {}

    /**
     * copy constructor (and copying with allocator)
     * Constructs a container with a copy of each of the elements in x.
     */
    set(const set& x): set(x, x.get_allocator()) {}

    set(const set& x, const allocator_type& alloc): 
        less_(x.less_), node_alloc_(alloc)
    {
        initialize();
        tree_->root = clone_tree(x.tree_, x.tree_->root, &tree_->nil);
    }

    /**
     * move constructor (and moving with allocator)
     * Constructs a container that acquires the elements of x.
     * If alloc is specified and is different from x's allocator, the elements are moved. 
     * Otherwise, no elements are constructed (their ownership is directly transferred).
     * x is left in an unspecified but valid state.
     */
    set(set&& x): less_(std::move(x.less_)), node_alloc_(std::move(x.node_alloc_))
    {
        initialize();
        tree_ = x.tree_;
        x.initialize();
    }

    set(set&& x, const allocator_type& alloc): less_(std::move(x.less_)), node_alloc_(alloc)
    {
        initialize();
        try
        {
            tree_->root = move_tree(x.tree_, x.tree_->root, &tree_->nil);
            x.clear();
        }
        catch (...)
        {
            x.clear();
            finalize();
            throw;
        }
    }

    /**
     * Copy container content
     * Assigns new contents to the container, replacing its current content.
     */
    set& operator=(const set& x)
    {
        if (this == &x) {
            return *this;
        }

        tree_type *new_tree = new tree_type{};
        tree_init(new_tree);
        new_tree->root = clone_tree(x.tree_, x.tree_->root, &new_tree->nil);
        finalize();
        tree_ = new_tree;
        return *this;
    }

    set& operator=(set&& x)
    {
        if (this == &x) {
            return *this;
        }

        set(std::move(x)).swap(*this);
        return *this;
    }

    set& operator=(std::initializer_list<value_type> il)
    {
        set(il).swap(*this);
        return *this;
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
        link_type* x = tree_minimum(tree_, tree_->root);
        return iterator(tree_, x);
    }

    const_iterator begin() const noexcept
    {
        link_type* x = tree_minimum(tree_, tree_->root);
        return const_iterator(tree_, x);
    }

    /**
     * Return iterator to end
     * Returns an iterator referring to the past-the-end element in the set container.
     */
    iterator end() noexcept
    {
        return iterator(tree_, &tree_->nil);
    }

    const_iterator end() const noexcept
    {
        return const_iterator(tree_, &tree_->nil);
    }

    /**
     * Return const_iterator to beginning
     * Returns a const_iterator pointing to the first element in the container.
     */
    const_iterator cbegin() const noexcept
    {
        link_type* x = tree_minimum(tree_, tree_->root);
        return const_iterator(tree_, x);
    }

    /**
     * Return const_iterator to end
     * Returns a const_iterator pointing to the past-the-end element in the container.
     */
    const_iterator cend() const noexcept
    {
        return const_iterator(tree_, &tree_->nil);
    }

    /**
     * Return reverse iterator to reverse beginning
     * Returns a reverse iterator pointing to the last element in the container (i.e., its reverse beginning).
     *
     * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
     */
    reverse_iterator rbegin() noexcept
    {
        link_type* x = tree_maximum(tree_, tree_->root);
        return reverse_iterator(tree_, x);
    }

    const_reverse_iterator rbegin() const noexcept
    {
        link_type* x = tree_maximum(tree_, tree_->root);
        return const_reverse_iterator(tree_, x);
    }

    /**
     * Return reverse iterator to reverse end
     * Returns a reverse iterator pointing to the theoretical element right before the first element in the set container (which is considered its reverse end).
     */
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(tree_, &tree_->nil);
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(tree_, &tree_->nil);
    }

    /**
     * Test whether container is empty
     * Returns whether the set container is empty (i.e. whether its size is 0).
     */
    bool empty() const noexcept
    {
        return tree_is_empty(tree_);
    }

    /**
     * Return container size
     * Returns the number of elements in the set container.
     */
    size_type size() const noexcept
    {
        return tree_size(tree_, tree_->root);
    }

    /**
     * Return maximum size
     * Returns the maximum number of elements that the set container can hold.
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(node_type);
    }

    /**
     * Insert element
     * Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.
     */
    std::pair<iterator, bool> insert(const value_type& val)
    {
        link_type *node = create_node(val);
        std::pair<link_type*, bool> ret = insert(tree_, node);
        if (ret.second == false) {
            destroy_node(node);
        }
        return std::make_pair(iterator(tree_, ret.first), ret.second);
    }

    std::pair<iterator,bool> insert(value_type&& val)
    {
        link_type *node = create_node(std::move(val));
        std::pair<link_type*, bool> ret = insert(tree_, node);
        if (ret.second == false) {
            destroy_node(node);
        }
        return std::make_pair(iterator(tree_, ret.first), ret.second);
    }

    iterator insert(const_iterator position, const value_type& val)
    {
        return insert(val).first;
    }

    iterator insert(const_iterator position, value_type&& val)
    {
        return insert(std::move(val)).first;
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        while (first != last) {
            insert(*first++);
        }
    }

    void insert(std::initializer_list<value_type> il)
    {
        insert(il.begin(), il.end());
    }

    /**
     * Erase elements
     * Removes from the set container either a single element or a range of elements ([first,last)).
     */
    iterator erase(const_iterator position)
    {
        assert(position != end());
        link_type *node = position.link;
        iterator next(tree_, node);
        ++next;
        tree_delete(tree_, node);
        destroy_node(node);
        return next;
    }

    size_type erase(const value_type& val)
    {
        iterator it = find(val);
        if (it = end())
            return 0;
        erase(it);
        return 1;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        const_iterator it = first;
        while (it != last) {
            it = erase(it);
        }
        return iterator(tree_, last.link);
    }

    /**
     * Swap content
     * Exchanges the content of the container by the content of x, which is another set of the same type. Sizes may differ.
     */
    void swap(set& x)
    {
        using std::swap;
        swap(tree_, x.tree_);
        swap(less_, x.less_);
        swap(node_alloc_, x.node_alloc_);
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
        link_type *node = create_node(std::forward<Args>(args)...);
        std::pair<link_type*, bool> ret = insert(tree_, node);
        if (ret.second == false) {
            destroy_node(node);
        }
        return std::make_pair(iterator(tree_, ret.first), ret.second);
    }

    template <typename... Args>
    iterator emplace_hint(const_iterator position, Args&&... args)
    {
        return emplace(std::forward<Args>(args)...).first;
    }

    /**
     * Return comparison object
     * Returns a copy of the comparison object used by the container.
     */
    key_compare key_comp() const
    {
        return less_;
    }

    /**
     * Return comparison object
     * Returns a copy of the comparison object used by the container.
     */
    value_compare value_comp() const
    {
        return less_;
    }

    /**
     * Get iterator to element
     * Searches the container for an element equivalent to val and returns an iterator to it if found, 
     * otherwise it returns an iterator to set::end.
     */
    const_iterator find(const value_type& val) const
    {
        return const_iterator(tree_, find(tree_, val));
    }

    iterator find(const value_type& val)
    {
        return iterator(tree_, find(tree_, val));
    }

    /**
     * Count elements with a specific value
     * Searches the container for elements equivalent to val and returns the number of matches.
     */
    size_type count(const value_type& val) const
    {
        return find(tree_, val) == &tree_->nil ? 0 : 1;
    }

    /**
     * Return iterator to lower bound
     * Returns an iterator pointing to the first element in the container which is not considered to go before val (i.e., either it is equivalent or goes after).
     */
    iterator lower_bound(const value_type& val)
    {
        return iterator(tree_, lower_bound(tree_, val));
    }

    const_iterator lower_bound(const value_type& val) const
    {
        return const_iterator(tree_, lower_bound(tree_, val));
    }

    /**
     * Return iterator to upper bound
     * Returns an iterator pointing to the first element in the container which is considered to go after val.
     */
    iterator upper_bound(const value_type& val)
    {
        return iterator(tree_, upper_bound(tree_, val));
    }

    const_iterator upper_bound(const value_type& val) const
    {
        return const_iterator(tree_, upper_bound(tree_, val));
    }

    /**
     * Get range of equal elements
     * Returns the bounds of a range that includes all the elements in the container that are equivalent to val.
     *
     * Because all elements in a set container are unique, the range returned will contain a single element at most.
     */
    std::pair<iterator,iterator> equal_range(const value_type& val)
    {
        auto ret = equal_range(tree_, val);
        return std::make_pair(iterator(tree_, ret.first), iterator(tree_, ret.second));
    }

    std::pair<const_iterator,const_iterator> equal_range(const value_type& val) const
    {
        auto ret = equal_range(tree_, val);
        return std::make_pair(const_iterator(tree_, ret.first), const_iterator(tree_, ret.second));
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
    const value_type& get_value(link_type* node) const
    {
        return *static_cast<node_type*>(node)->valptr();
    }

    node_type* get_node()
    {
        return node_alloc_.allocate(1);
    }

    void put_node(node_type* node)
    {
        node_alloc_.deallocate(node, 1);
    }

    template <typename ...Args>
    link_type* create_node(Args&&... args)
    {
        node_type* node = get_node();
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
        return static_cast<link_type*>(node);
    }

    void destroy_node(link_type* link)
    {
        node_type* node = static_cast<node_type*>(link);
        node->valptr()->~T();
        node_alloc_.deallocate(node, 1);
    }

    void initialize()
    {
        tree_ = new tree_type{};
        tree_init(tree_);
    }

    void finalize()
    {
        destroy_tree(tree_->root, &tree_->nil);
        delete tree_;
    }

    void destroy_tree(link_type* root, link_type* nil)
    {
        if (root == nil)
            return;

        destroy_tree(root->left, nil);
        destroy_tree(root->right, nil);
        destroy_node(root);
    }

    std::pair<link_type*, bool> insert(tree_type* tree, link_type* z)
    {
        const value_type& z_val = get_value(z);
        link_type* y = &tree->nil;
        link_type* x = tree->root;
        while (x != &tree->nil) {
            y = x;
            const value_type& x_val = get_value(x);
            if (less_(z_val, x_val)) {          // z->val < x->val
                x = x->left;
            } else if (less_(x_val, z_val)) {     // x->val < z->val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, false);
            }
        }

        if (y == &tree->nil) {
            tree_set_root(tree, z);
        } else if (less_(get_value(z), get_value(y))) {
            tree_set_left_child(y, z);
        } else {
            tree_set_right_child(y, z);
        }

        z->left = &tree->nil;
        z->right = &tree->nil;
        z->color = red_black::tree_node_color_t::kRed;
        tree_insert_fixup(tree, z);
        return std::make_pair(z, true);
    }

    link_type* find(tree_type* tree, const value_type& val) const 
    {
        link_type* x = tree->root;
        while (x != &tree->nil) {
            const value_type& x_val = *static_cast<node_type*>(x)->valptr();
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

    link_type* lower_bound(tree_type* tree, const value_type& val) const 
    {
        link_type* y = &tree->nil;
        link_type* x = tree->root;
        while (x != &tree->nil) {
            y = x;
            const value_type& x_val = get_value(x);
            if (less_(val, x_val)) {          // z->val < x->val
                x = x->left;
            } else if (less_(x_val, val)) {     // x->val < z->val
                x = x->right;
            } else {    // val == x->val
                return x;
            }
        }

        if (less_(get_value(y), val)) {
            return tree_successor(tree, y);
        } else {
            return y;
        }
    }

    link_type* upper_bound(tree_type* tree, const value_type& val) const 
    {
        link_type* y = &tree->nil;
        link_type* x = tree->root;
        while (x != &tree->nil) {
            y = x;
            const value_type& x_val = get_value(x);
            if (less_(val, x_val)) {          // z->val < x->val
                x = x->left;
            } else if (less_(x_val, val)) {     // x->val < z->val
                x = x->right;
            } else {    // val == x->val
                return tree_successor(tree, x);
            }
        }

        if (less_(get_value(y), val)) {
            return tree_successor(tree, y);
        } else {
            return y;
        }
    }

    std::pair<link_type*,link_type*> equal_range(tree_type* tree, const value_type& val) const
    {
        link_type* y = &tree->nil;
        link_type* x = tree->root;
        while (x != &tree->nil) {
            y = x;
            const value_type& x_val = get_value(x);
            if (less_(val, x_val)) {          // z->val < x->val
                x = x->left;
            } else if (less_(x_val, val)) {     // x->val < z->val
                x = x->right;
            } else {    // val == x->val
                return std::make_pair(x, tree_successor(tree, x));
            }
        }

        if (less_(get_value(y), val)) {
            link_type *z = tree_successor(tree, y);
            return std::make_pair(z, z);
        } else {
            return std::make_pair(y, y);
        }
    }

    link_type* clone_tree(tree_type* tree, link_type* root, link_type* new_nil) 
    {
        if (root == &tree->nil)
            return new_nil;

        link_type* left = NULL;
        link_type* right = NULL;
        link_type* node = NULL;

        try 
        {
            // clone left child tree
            left = clone_tree(tree, root->left, new_nil);

            // clone right child tree
            right = clone_tree(tree, root->right, new_nil);

            // clone root node
            node = create_node(*static_cast<const node_type*>(root)->valptr());
            node->color = root->color;
            node->parent = new_nil;
            tree_set_left_child(node, left);
            tree_set_right_child(node, right);
            return node;
        }
        catch (...)
        {
            if (left) destroy_tree(left, new_nil);
            if (right) destroy_tree(right, new_nil);
            if (node) destroy_node(node);
            throw;
        }
    }

    link_type* move_tree(tree_type* tree, link_type* root, link_type* new_nil) 
    {
        if (root == &tree->nil)
            return new_nil;


        link_type* left = NULL;
        link_type* right = NULL;
        link_type* node = NULL;

        try 
        {
            // move left child tree
            left = move_tree(tree, root->left, new_nil);

            // move right child tree
            right = move_tree(tree, root->right, new_nil);

            // move root node
            node = create_node(std::move(*static_cast<const node_type*>(root)->valptr()));
            node->color = root->color;
            node->parent = new_nil;
            tree_set_left_child(node, left);
            tree_set_right_child(node, right);
            return node;
        }
        catch (...)
        {
            if (left) destroy_tree(left, new_nil);
            if (right) destroy_tree(right, new_nil);
            if (node) destroy_node(node);
            throw;
        }
    }
};

template <typename T, typename Compare, typename Alloc>
inline
bool operator==(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Compare, typename Alloc>
inline
bool operator!=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Compare, typename Alloc>
inline
bool operator<(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename T, typename Compare, typename Alloc>
inline
bool operator>(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return (rhs < lhs);
}

template <typename T, typename Compare, typename Alloc>
inline
bool operator<=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(lhs > rhs);
}

template <typename T, typename Compare, typename Alloc>
inline
bool operator>=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(lhs < rhs);
}

} // namespace Hx

#endif // MINI_STL_SET_INC
