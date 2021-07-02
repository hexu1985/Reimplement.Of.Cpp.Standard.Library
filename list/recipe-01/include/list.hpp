// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_LIST_INC
#define MINI_STL_LIST_INC

#include "list_impl.h"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <functional>
#include <algorithm>
#include <initializer_list>

namespace mini_stl {

/**
 * A helper node class for list.
 * This is just a linked list with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct list_node: public double_linked::list_node_t {
    // raw storage buffer for type T
    typename std::aligned_storage<sizeof (T), std::alignment_of<T>::value>::type storage;

    T *valptr() noexcept
    {
        return static_cast<T *>(static_cast<void *>(&storage));
    }

    const T *valptr() const noexcept
    {
        return static_cast<const T *>(static_cast<const void *>(&storage));
    }
};

template <typename T>
struct list_iterator {
    typedef double_linked::list_node_t link_type;
    link_type *link = nullptr;

    typedef list_iterator<T> this_type;
    typedef list_node<T> node_type;

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    list_iterator() = default;

    explicit list_iterator(link_type *link_): link(link_) {}

    reference operator *() const 
    {
        return *static_cast<node_type *>(link)->valptr();
    }

    pointer operator ->() const 
    {
        return static_cast<node_type *>(link)->valptr();
    }

    this_type &operator ++()    // prefix increment
    {
        next();
        return *this;
    }

    this_type operator ++(int)    // postfix increment
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type &operator --()    // prefix decrement
    {
        prev();
        return *this;
    }

    this_type operator --(int)    // postfix decrement
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
        link = link->next;
    }

    void prev()
    {
        link = link->prev;
    }
};

template <typename T>
struct list_const_iterator {
    typedef const double_linked::list_node_t link_type;
    link_type *link = nullptr;

    typedef list_const_iterator<T> this_type;
    typedef const list_node<T> node_type;
    typedef list_iterator<T> iterator;

    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    list_const_iterator() = default;

    explicit list_const_iterator(link_type *link_): link(link_) {}

    list_const_iterator(const iterator &iter): link(iter.link) {}

    reference operator *() const 
    {
        return *static_cast<node_type *>(link)->valptr();
    }

    pointer operator ->() const 
    { 
        return static_cast<node_type *>(link)->valptr();
    }

    this_type &operator ++()    // prefix increment
    {
        next();
        return *this;
    }

    this_type operator ++(int)    // postfix increment
    {
        this_type tmp(*this);
        next();
        return tmp;
    }

    this_type &operator --()    // prefix decrement
    {
        prev();
        return *this;
    }

    this_type operator --(int)    // postfix decrement
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
bool operator ==(const list_iterator<T> &x, const list_const_iterator<T> &y)
{
    return (x.link == y.link);
}

/**
 * List iterator inequality comparison.
 */
template <typename T>
inline 
bool operator !=(const list_iterator<T> &x, const list_const_iterator<T> &y)
{
    return !(x == y);
}

/**
 * List
 */
template <typename T, typename Alloc = std::allocator<T> >
class list {
    typedef double_linked::list_t list_type;
    typedef double_linked::list_node_t link_type;
    typedef list_node<T> node_type;
    typedef typename Alloc::template rebind<node_type>::other node_alloc_type;

    node_alloc_type node_alloc_;
    list_type lst_;

public:
    /* Type Definitions of Lists */
    typedef T value_type;
    typedef Alloc allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef list_iterator<T> iterator;
    typedef list_const_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    /**
     * empty container constructor (default constructor)
     * Constructs an empty container, with no elements.
     */
    list(): list(allocator_type()) {}

    explicit list(const allocator_type &alloc): node_alloc_(alloc)
    {
        initialize();
    }

    /**
     * fill constructor
     * Constructs a container with n elements. 
     * Each element is a copy of val (if provided).
     */
    explicit list(size_type n): list(n, value_type(), allocator_type()) {}

    list(size_type n, const value_type &val,
        const allocator_type &alloc = allocator_type()): node_alloc_(alloc)
    {
        initialize();
        try
        {
            fill(n, val);
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    /**
     * range constructor
     * Constructs a container with as many elements as the range [first,last), 
     * with each element emplace-constructed from its corresponding element 
     * in that range, in the same order.
     */
    template <typename InputIterator, typename = typename
        std::enable_if<!std::is_integral<InputIterator>::value>::type>
    list(InputIterator first, InputIterator last,
        const allocator_type &alloc = allocator_type()): node_alloc_(alloc)
    {
        initialize();
        try 
        {
            copy_from(first, last);
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    /**
     * copy constructor (and copying with allocator)
     * Constructs a container with a copy of each of the elements in x, 
     * in the same order.
     */
    list(const list &x): node_alloc_(x.node_alloc_)
    {
        initialize();
        try
        {
            copy_from(x.begin(), x.end());
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    list(const list &x, const allocator_type &alloc): node_alloc_(alloc)
    {
        initialize();
        try
        {
            copy_from(x.begin(), x.end());
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    /**
     * move constructor (and moving with allocator)
     * Constructs a container that acquires the elements of x.
     * If alloc is specified and is different from x's allocator, 
     * the elements are moved. Otherwise, no elements are constructed 
     * (their ownership is directly transferred).
     * x is left in an unspecified but valid state.
     */
    list(list &&x): node_alloc_(std::move(x.node_alloc_))
    {
        initialize();
        list_transfer_range(list_nil(&lst_), list_head(&x.lst_), list_tail(&x.lst_));
    }

    list(list &&x, const allocator_type &alloc): node_alloc_(alloc)
    {
        initialize();
        try
        {
            move_from(x.begin(), x.end());
        }
        catch (...)
        {
            finalize();
            throw;
        }
        x.clear();
    }

    /**
     * initializer list constructor
     * Constructs a container with a copy of each of the elements in il, 
     * in the same order.
     */
    list(std::initializer_list<value_type> il,
        const allocator_type &alloc = allocator_type()): node_alloc_(alloc)
    {
        initialize();
        try
        {
            copy_from(il.begin(), il.end());
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    /**
     * List destructor
     * Destroys the container object.
     */
    ~list() { finalize(); }

    /**
     * Assign content
     * Assigns new contents to the container, replacing its current contents, 
     * and modifying its size accordingly.
     */

    /**
     * The copy assignment 
     * copies all the elements from x into the container 
     * (with x preserving its contents).
     */
    list &operator =(const list &x)
    {
        if (this == &x)
            return *this;

        copy_from(x.begin(), x.end());
        return *this;
    }

    /**
     * The move assignment moves the elements of x into the container 
     * (x is left in an unspecified but valid state).
     */
    list &operator =(list &&x)
    {
        using std::swap;

        if (this == &x)
            return *this;

        this->clear();
        swap_data(x);
        swap(node_alloc_, x.node_alloc_);
        return *this;
    }

    /**
     * The initializer list assignment copies the elements of il into 
     * the container.
     */
    list &operator =(std::initializer_list<value_type> il)
    {
        copy_from(il.begin(), il.end());
        return *this;
    }

    /**
     * Return iterator to beginning
     * Returns an iterator pointing to the first element in the list container.
     */
    iterator begin() noexcept
    {
        return iterator(list_head(&lst_));
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(list_head(&lst_));
    }

    /**
     * Return iterator to end
     * Returns an iterator referring to the past-the-end element 
     * in the list container.
     */
    iterator end() noexcept
    {
        return iterator(list_nil(&lst_));
    }

    const_iterator end() const noexcept
    {
        return const_iterator(list_nil(&lst_));
    }

    /**
     * Return reverse iterator to reverse beginning
     * Returns a reverse iterator pointing to the last element in the container 
     * (i.e., its reverse beginning).
     */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept 
    {
        return const_reverse_iterator(end()); 
    }

    /**
     * Return reverse iterator to reverse end
     * Returns a reverse iterator pointing to the theoretical element preceding 
     * the first element in the container (which is considered its reverse end).
     */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept 
    {
        return const_reverse_iterator(begin()); 
    }

    /**
     * Return const_iterator to beginning
     * Returns a const_iterator pointing to the first element in the container.
     */
    const_iterator cbegin() const noexcept
    {
        return const_iterator(list_head(&lst_));
    }

    /**
     * Return const_iterator to end
     * Returns a const_iterator pointing to the past-the-end element 
     * in the container.
     * The value returned shall not be dereferenced.
     */
    const_iterator cend() const noexcept
    {
        return const_iterator(list_head(&lst_));
    }

    /**
     * Return const_reverse_iterator to reverse beginning
     * Returns a const_reverse_iterator pointing to the last element 
     * in the container (i.e., its reverse beginning).
     */
    const_reverse_iterator crbegin() const noexcept 
    {
        return const_reverse_iterator(end()); 
    }

    /**
     * Return const_reverse_iterator to reverse end
     * Returns a const_reverse_iterator pointing to the theoretical element 
     * preceding the first element in the container 
     * (which is considered its reverse end).
     */
    const_reverse_iterator crend() const noexcept 
    {
        return const_reverse_iterator(begin()); 
    }

    /**
     * Test whether container is empty
     * Returns whether the list container is empty (i.e. whether its size is 0).
     */
    bool empty() const noexcept { return list_is_empty(&lst_); }

    /**
     * Return size
     * Returns the number of elements in the list container.
     */
    size_type size() const noexcept 
    {
        return list_size(&lst_); 
    }

    /**
     * Return maximum size
     * Returns the maximum number of elements that the list container can hold.
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof (node_type);
    }

    /**
     * Access first element
     * Returns a reference to the first element in the list container.
     */
    reference front()
    {
        assert(!empty());
        return get_ref(list_head(&lst_));
    }

    const_reference front() const
    {
        assert(!empty());
        return get_ref(list_head(&lst_));
    }

    /**
     * Access last element
     * Returns a reference to the last element in the list container.
     */
    reference back()
    {
        assert(!empty());
        return get_ref(list_tail(&lst_));
    }

    const_reference back() const
    {
        assert(!empty());
        return get_ref(list_tail(&lst_));
    }

    /**
     * Assign new content to container
     * Assigns new contents to the list container, replacing its current 
     * contents, and modifying its size accordingly.
     */

    /**
     * In the range version, the new contents are elements constructed from 
     * each of the elements in the range between first and last, 
     * in the same order.
     */
    template <typename InputIterator, typename = typename
        std::enable_if<!std::is_integral<InputIterator>::value>::type>
    void assign(InputIterator first, InputIterator last)
    {
        copy_from(first, last); 
    }

    /**
     * In the fill version, the new contents are n elements, 
     * each initialized to a copy of val.
     */
    void assign(size_type n, const value_type &val) { fill(n, val); }

    /**
     * In the initializer list version, the new contents are copies of 
     * the values passed as initializer list, in the same order.
     */
    void assign(std::initializer_list<value_type> il)
    {
        copy_from(il.begin(), il.end());
    }

    /**
     * Construct and insert element at beginning
     * Inserts a new element at the beginning of the list, right before its 
     * current first element. This new element is constructed in place 
     * using args as the arguments for its construction.
     */
    template <typename ... Args>
    void emplace_front(Args &&... args)
    {
        list_insert(list_head(&lst_), create_node(std::forward<Args>(args) ...));
    }

    /**
     * Insert element at beginning
     * Inserts a new element at the beginning of the list, 
     * right before its current first element. 
     * The content of val is copied (or moved) to the inserted element.
     */
    void push_front(const value_type &val)
    {
        list_insert(list_head(&lst_), create_node(val));
    }

    void push_front(value_type &&val)
    {
        list_insert(list_head(&lst_), create_node(std::forward<value_type>(val)));
    }

    /**
     * Delete first element
     * Removes the first element in the list container, 
     * effectively reducing its size by one.
     */
    void pop_front()
    {
        assert(!empty());
        link_type *link = list_head(&lst_);
        list_delete(link);
        destroy_node(link);
    }

    /**
     * Construct and insert element at the end
     * Inserts a new element at the end of the list, right after its current 
     * last element. This new element is constructed in place 
     * using args as the arguments for its construction.
     */
    template <typename ... Args>
    void emplace_back(Args &&... args)
    {
        list_insert(list_nil(&lst_), create_node(std::forward<Args>(args) ...));
    }

    /**
     * Add element at the end
     * Adds a new element at the end of the list container, after its current 
     * last element. The content of val is copied to the new element.
     */
    void push_back(const value_type &val)
    {
        list_insert(list_nil(&lst_), create_node(val));
    }

    /**
     * Add element at the end
     * Adds a new element at the end of the list container, after its current 
     * last element. The content of val is moved to the new element.
     */
    void push_back(value_type &&val)
    {
        list_insert(list_nil(&lst_), 
            create_node(std::forward<value_type>(val)));
    }

    /**
     * Delete last element
     * Removes the last element in the list container, 
     * effectively reducing the container size by one.
     */
    void pop_back()
    {
        assert(!empty());
        link_type *link = list_tail(&lst_);
        list_delete(link);
        destroy_node(link);
    }

    template <typename ... Args>
    iterator emplace(const_iterator position, Args &&... args)
    {
        link_type *link = create_node(std::forward<Args>(args) ...);
        list_insert((link_type *) position.link, link);
        return iterator(link);
    }

    /**
     * Insert elements
     * The container is extended by inserting new elements 
     * before the element at the specified position.
     */
    iterator insert(const_iterator position, const value_type &val)
    {
        link_type *link = create_node(val);
        list_insert((link_type *) position.link, link);
        return iterator(link);
    }

    iterator insert(const_iterator position, value_type &&val)
    {
        link_type *link = create_node(std::forward<value_type>(val));
        list_insert((link_type *) position.link, link);
        return iterator(link);
    }

    iterator insert(const_iterator position, size_type n, const value_type &val)
    {
        link_type *link = const_cast<link_type *>(position.link);
        link_type *keep = link;
        for (size_type i = 0; i < n; ++i) {
            link_type *link = create_node(val);
            if (i == 0) {
                keep = link;
            }
            list_insert((link_type *) position.link, link);
        }
        return iterator(keep);
    }

    template <typename InputIterator, typename = typename
        std::enable_if<!std::is_integral<InputIterator>::value>::type>
    iterator insert(const_iterator position, InputIterator first, 
        InputIterator last)
    {
        link_type *keep = const_cast<link_type *>(position.link);
        for (InputIterator iter = first; iter != last; ++iter) {
            link_type *link = create_node(*iter);
            if (iter == first) {
                keep = link;
            }
            list_insert((link_type *) position.link, link);
        }
        return iterator(keep);
    }

    iterator insert(const_iterator position, 
        std::initializer_list<value_type> il)
    {
        return insert(position, il.begin(), il.end());
    }

    /**
     * Erase elements
     * Removes from the list container either a single element (position) 
     * or a range of elements ([first,last)).
     */
    iterator erase(const_iterator position)
    {
        link_type *keep = (link_type *) position.link->next;
        list_delete((link_type *) position.link);
        destroy_node((link_type *) position.link);
        return iterator(keep);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last) return iterator((link_type *) last.link);

        list_delete_range((link_type *) first.link, (link_type *) last.link->prev);
        range_destroy((link_type *) first.link, (link_type *) last.link);
        return iterator((link_type *) last.link);
    }
    
    /**
     * Swap content
     * Exchanges the content of the container by the content of x, 
     * which is another list of the same type. Sizes may differ.
     */
    void swap(list &x)
    {
        using std::swap;
        swap_data(x);
        swap(node_alloc_, x.node_alloc_);
    }

    /**
     * Change size
     * Resizes the container so that it contains n elements.
     */
    void resize(size_type n)
    {
        resize(n, value_type());
    }

    void resize(size_type n, const value_type &val)
    {
        link_type *link = lst_.nil.next;
        size_type i = 0;
        link_type *nil = &lst_.nil;
        for ( ; link != nil && i < n; ++i) {
            link = link->next;
        }
        if (link == nil) {
            for ( ; i < n; ++i) {
                list_insert(nil, create_node(val));
            }
        } else {
            list_delete_range(link, nil->prev);
            range_destroy(link, nil);
        }
    }

    /**
     * Clear content
     * Removes all elements from the list container (which are destroyed), 
     * and leaving the container with a size of 0.
     */
    void clear() noexcept
    {
        finalize();
        initialize();
    }

    /**
     * Transfer elements from list to list
     * Transfers elements from x into the container, inserting them at position.
     */
    void splice(const_iterator position, list &x)
    {
        if (x.empty()) return;

        list_transfer_range((link_type *) position.link, 
            list_head(&x.lst_), list_tail(&x.lst_));
    }

    void splice(const_iterator position, list &x, const_iterator i)
    {
        if (i == x.end()) return;

        list_transfer((link_type *) position.link, (link_type *) i.link);
    }

    void splice(const_iterator position, list &x, 
        const_iterator first, const_iterator last)
    {
        if (first == last) return;

        list_transfer_range((link_type *) position.link, 
            (link_type *) first.link, (link_type *) last.link->prev);
    }

    void splice(const_iterator position, list &&x)
    {
        if (x.empty()) return;

        list_transfer_range((link_type *) position.link, 
            list_head(&x.lst_), list_tail(&x.lst_));
    }

    void splice(const_iterator position, list &&x, const_iterator i)
    {
        if (i == x.end()) return;

        list_transfer((link_type *) position.link, (link_type *) i.link);
    }

    void splice(const_iterator position, list &&x, 
        const_iterator first, const_iterator last)
    {
        if (first == last) return;

        list_transfer_range((link_type *) position.link, 
            (link_type *) first.link, (link_type *) last.link->prev);
    }

    /**
     * Remove elements with specific value
     * Removes from the container all the elements that compare equal to val. 
     * This calls the destructor of these objects and reduces the container 
     * size by the number of elements removed.
     */
    void remove(const value_type &val)
    {
        remove_if([&val](const value_type &node_val) { return val == node_val; });
    }

    /**
     * Remove elements fulfilling condition
     * Removes from the container all the elements for which Predicate pred 
     * returns true. This calls the destructor of these objects and reduces 
     * the container size by the number of elements removed.
     */
    template <typename Predicate>
    void remove_if(Predicate pred)
    {
        auto pos = static_cast<node_type *>(list_head(&lst_));
        auto nil = static_cast<node_type *>(list_nil(&lst_));
        while (pos != nil) {
            link_type *next = pos->next;
            if (pred(*pos->valptr())) {
                list_delete(pos);
                destroy_node(pos);
            }
            pos = static_cast<node_type *>(next);
        }
    }

    /**
     * Remove duplicate values
     * removes all but the first element from every consecutive group of 
     * equal elements in the container.
     */
    void unique()
    {
        unique(std::equal_to<value_type>());
    }

    template <typename BinaryPredicate>
    void unique(BinaryPredicate binary_pred)
    {
        auto pos = static_cast<node_type *>(list_head(&lst_));
        auto nil = static_cast<node_type *>(list_nil(&lst_));
        while (pos != nil) {
            node_type *next = static_cast<node_type *>(pos->next);
            while (next != nil && binary_pred(*pos->valptr(), *next->valptr())) {
                list_delete(next);
                destroy_node(next);
                next = static_cast<node_type *>(pos->next);
            }
            pos = next;
        }
    }

    /**
     * Merge sorted lists
     * Merges x into the list by transferring all of its elements at 
     * their respective ordered positions into the container 
     * (both containers shall already be ordered).
     */
    void merge(list &x)
    {
        merge(x, std::less<value_type>());
    }

    template <typename Compare>
    void merge(list &x, Compare comp)
    {
        auto dst_pos = static_cast<node_type *>(list_head(&lst_));
        auto dst_nil = static_cast<node_type *>(list_nil(&lst_));
        auto src_pos = static_cast<node_type *>(list_head(&x.lst_));
        auto src_nil = static_cast<node_type *>(list_nil(&x.lst_));
        merge(dst_pos, dst_nil, src_pos, src_nil, comp);
    }

    void merge(list &&x)
    {
        merge(std::forward(x), std::less<value_type>());
    }

    template <typename Compare>
    void merge(list &&x, Compare comp)
    {
        auto dst_pos = static_cast<node_type *>(list_head(&lst_));
        auto dst_nil = static_cast<node_type *>(list_nil(&lst_));
        auto src_pos = static_cast<node_type *>(list_head(&x.lst_));
        auto src_nil = static_cast<node_type *>(list_nil(&x.lst_));
        merge(dst_pos, dst_nil, src_pos, src_nil, comp);
    }

    template <typename Compare>
    void merge(node_type *dst_pos, node_type *dst_nil,
        node_type *src_pos, node_type *src_nil, Compare comp)
    {
        while (dst_pos != dst_nil && src_pos != src_nil) {
            // find first greater than src_pos in dst list, 
            // is the insert pos in dst list
            while (dst_pos != dst_nil && !comp(*src_pos->valptr(), *dst_pos->valptr()))
                dst_pos = static_cast<node_type *>(dst_pos->next);
            if (dst_pos == dst_nil) // nofound
                break;

            // find first pos greater than dst_pos in src list,
            // range [src_beg, src_pos) is the transfer range
            auto src_beg = src_pos;
            while (src_pos != src_nil && !comp(*dst_pos->valptr(), *src_pos->valptr()))
                src_pos = static_cast<node_type *>(src_pos->next);

            // transfer [src_beg, src_pos) insert front dst_pos
            list_transfer_range(dst_pos, src_beg, src_pos->prev);

            // because dst_pos's key < src_pos's key, move dst_pos to next
            dst_pos = static_cast<node_type *>(dst_pos->next);
        }

        // transfer src list's remainders to dst list tail if need
        if (dst_pos == dst_nil && src_pos != src_nil) {
            list_transfer_range(dst_nil, src_pos, src_nil->prev);
        }
    }

    /**
     * Sort elements in container
     * Sorts the elements in the list, altering their position within 
     * the container.
     */
    void sort()
    {
        sort(std::less<value_type>());
    }

#ifdef USE_STD_SORT
    template <typename Compare>
    void sort(Compare comp)
    {
        size_type n = size();
        if (n < 2) return;

        link_type **array = new link_type *[n];

        link_type *link = list_head(&lst_); 
        link_type *nil = list_nil(&lst_);
        for (link_type **ptr = array; link != nil; link = link->next) {
            *ptr++ = link;
        }
        std::sort(array, array+n, make_compare(comp));

        initialize();
        for (link_type **ptr = array, **end = array+n; ptr != end; ) {
            list_insert(nil, *ptr++);
        }

        delete [] array;
    }
#else
    template <typename Compare>
    void sort(Compare comp)
    {
        link_type *nil = list_nil(&lst_);

        link_type *i = nil->next->next;
        while (i != nil) {
            link_type *next = i->next;
            link_type *j = i->prev;
            while (j != nil && comp(*static_cast<node_type *>(i)->valptr(), *static_cast<node_type *>(j)->valptr())) {
                j = j->prev;
            }
            if (j != i->prev) {
                list_delete(i);
                list_insert(j->next, i);
            }
            i = next;
        }
    }
#endif

    /**
     * Reverse the order of elements
     * Reverses the order of the elements in the list container.
     */
    void reverse() noexcept
    {
        list_reverse(list_head(&lst_), list_nil(&lst_));
    }

    /**
     * Get allocator
     * Returns a copy of the allocator object associated with 
     * the list container.
     */
    allocator_type get_allocator() const noexcept
    {
        return allocator_type(node_alloc_);
    }

private:
    void swap_data(list &x)
    {
        list_swap(&lst_, &x.lst_);
    }

    void fill(size_type n, const value_type &val)
    {
        link_type *link = lst_.nil.next;
        link_type *nil = &lst_.nil;
        while ((link != nil) && (n > 0)) {
            *static_cast<node_type *>(link)->valptr() = val;
            n--;
            link = link->next;
        }

        if (link == nil) {    // list.size() < n
            while (n > 0) {
                list_insert(nil, create_node(val));
                n--;
            }
        } else if (n == 0) {    // list.size() > n
            list_delete_range(link, nil->prev);
            range_destroy(link, nil);
        } 

        // list.size() == n
    }

    template <typename InputIterator>
    void copy_from(InputIterator first, InputIterator last)
    {
        link_type *link = lst_.nil.next;
        link_type *nil = &lst_.nil;
        while ((link != nil) && (first != last)) {
            *static_cast<node_type *>(link)->valptr() = *first;
            ++first;
            link = link->next;
        }

        if (link == nil) {    // list.size() < distance(first, last)
            while (first != last) {
                list_insert(nil, create_node(*first++));
            }
        } else if (first == last) {    // list.size() > distance(first, last)
            list_delete_range(link, nil->prev);
            range_destroy(link, nil);
        } 

        // list.size() == distance(first, last)
    }

    template <typename InputIterator>
    void move_from(InputIterator first, InputIterator last)
    {
        link_type *link = lst_.nil.next;
        link_type *nil = &lst_.nil;
        while ((link != nil) && (first != last)) {
            *static_cast<node_type *>(link)->valptr() = std::move(*first);
            ++first;
            link = link->next;
        }

        if (link == nil) {    // list.size() < distance(first, last)
            while (first != last) {
                list_insert(nil, create_node(std::move(*first++)));
            }
        } else if (first == last) {    // list.size() > distance(first, last)
            list_delete_range(link, nil->prev);
            range_destroy(link, nil);
        } 

        // list.size() == distance(first, last)
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

    void range_destroy(link_type *first, link_type *last)
    {
        auto link = first;
        while (first != last) {
            link = first;
            first = first->next;
            destroy_node(link);
        }
    }

    static reference get_ref(link_type *link)
    {
        node_type *node = static_cast<node_type *>(link);
        return *node->valptr();
    }

    static const value_type &get_val(const link_type *link)
    {
        const node_type *node = static_cast<const node_type *>(link);
        return *node->valptr();
    }

    template <typename BinaryPredicate>
    struct compare_node {
        BinaryPredicate comp;

        explicit compare_node(const BinaryPredicate &comp_): comp(comp_) {}

        bool operator ()(const link_type *a, const link_type *b)
        {
            return comp(get_val(a), get_val(b));
        }
    };
    
    template <typename BinaryPredicate>
    compare_node<BinaryPredicate> make_compare(const BinaryPredicate &comp)
    {
        return compare_node<BinaryPredicate>(comp);
    }

    void initialize() 
    {
        list_init(&lst_); 
    }

    void finalize()
    {
        range_destroy(list_head(&lst_), list_nil(&lst_));
    }
};

/**
 * Relational operators for list
 * Performs the appropriate comparison operation between the list containers 
 * lhs and rhs.
 */
template <typename T, typename Alloc>
inline
bool operator ==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline
bool operator !=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline
bool operator <(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline
bool operator >(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    return (rhs < lhs);
}

template <typename T, typename Alloc>
inline
bool operator <=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    return !(lhs > rhs);
}

template <typename T, typename Alloc>
inline
bool operator >=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
    return !(lhs < rhs);
}

/**
 * Exchange contents of lists
 * The contents of container x are exchanged with those of y. 
 * Both container objects must be of the same type (same template parameters), 
 * although sizes may differ.
 */
template <typename T, typename Alloc>
inline
void swap(list<T, Alloc> &x, list<T, Alloc> &y)
{
    return x.swap(y);
}

} // namespace mini_stl

#endif // MINI_STL_LIST_INC

