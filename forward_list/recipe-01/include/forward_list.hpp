// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_FORWARD_LIST_INC
#define MINI_STL_FORWARD_LIST_INC

#include "forward_list_impl.h"

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
 * A helper node class for forward_list.
 * This is just a linked list node with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct forward_list_node: public single_linked::list_node_t {
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
 * A forward_list::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct forward_list_iterator {
    typedef single_linked::list_node_t link_type;
    link_type *link;

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef forward_list_iterator<T> this_type;
    typedef forward_list_node<T> node_type;

    forward_list_iterator(): link(nullptr) {}

    explicit forward_list_iterator(link_type *link_): link(link_) {}

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
        link = link->next;
    }
};

/**
 * A forward_list::const_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct forward_list_const_iterator {
    typedef const single_linked::list_node_t link_type;
    link_type *link;

    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef forward_list_const_iterator<T> this_type;
    typedef const forward_list_node<T> node_type;
    typedef forward_list_iterator<T> iterator;

    forward_list_const_iterator(): link(nullptr) {}

    explicit forward_list_const_iterator(link_type *link_): link(link_) {}

    forward_list_const_iterator(const iterator &iter): link(iter.link) {}

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
        link = link->next;
    }
};

/**
 * Forward list iterator equality comparison.
 */
template <typename T>
inline 
bool operator ==(const forward_list_iterator<T> &x,
    const forward_list_const_iterator<T> &y)
{
    return (x.link == y.link);
}

/**
 * Forward list iterator inequality comparison.
 */
template <typename T>
inline 
bool operator !=(const forward_list_iterator<T> &x,
    const forward_list_const_iterator<T> &y)
{
    return !(x == y);
}

/**
 * Forward list
 */
template <typename T, typename Alloc = std::allocator<T> >
class forward_list {
public:
    typedef T value_type;
    typedef Alloc allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef forward_list_iterator<T> iterator;
    typedef forward_list_const_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename iterator::difference_type difference_type;
    typedef size_t size_type;

private:
    typedef single_linked::list_t list_type;
    typedef single_linked::list_node_t link_type;
    typedef forward_list_node<T> node_type;
    typedef typename 
        allocator_type::template rebind<node_type>::other node_alloc_type;

    node_alloc_type node_alloc_;
    list_type lst_;

public:
    /**
     * empty container constructor (default constructor)
     * Constructs an empty container, with no elements.
     */
    forward_list(): forward_list(allocator_type()) {}

    explicit forward_list(const allocator_type &alloc): node_alloc_(alloc)
    {
        initialize();
    }

    /**
     * fill constructor
     * Constructs a container with n elements. Each element is a copy of val 
     * (if provided).
     */
    explicit forward_list(size_type n): 
        forward_list(n, value_type(), allocator_type()) {}

    forward_list(size_type n, const value_type &val,
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
    forward_list(InputIterator first, InputIterator last,
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
     * Constructs a container with a copy of each of the elements in fwdlst,
     * in the same order.
     */
    forward_list(const forward_list &x): node_alloc_(x.node_alloc_)
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

    forward_list(const forward_list &x, const allocator_type &alloc)
        : node_alloc_(alloc)
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
     * Constructs a container that acquires the elements of fwdlst.
     * If alloc is specified and is different from fwdlst's allocator, 
     * the elements are moved. Otherwise, no elements are constructed 
     * (their ownership is directly transferred).
     * fwdlst is left in an unspecified but valid state.
     */
    forward_list(forward_list &&x): node_alloc_(std::move(x.node_alloc_))
    {
        lst_ = x.lst_;
        x.initialize();
    }

    forward_list(forward_list &&x, const allocator_type &alloc): 
        node_alloc_(alloc)
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
    }

    /**
     * initializer list constructor
     * Constructs a container with a copy of each of the elements in il, 
     * in the same order.
     */
    forward_list(std::initializer_list<value_type> il,
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
    ~forward_list() { finalize(); }

    /**
     * Assign content
     * Assigns new contents to the container, replacing its current contents.
     */

    /**
     * The copy assignment copies all the elements from fwdlst into 
     * the container (with fwdlst preserving its contents).
     */
    forward_list &operator =(const forward_list &fwdlst)
    {
        if (this == &fwdlst)
            return *this;

        copy_from(fwdlst.begin(), fwdlst.end());
        return *this;
    }

    /**
     * The move assignment moves the elements of fwdlst into 
     * the container (x is left in an unspecified but valid state).
     */
    forward_list &operator =(forward_list &&x)
    {
        if (this == &x)
            return *this;

        this->swap(x);
        return *this;
    }

    /**
     * The initializer list assignment copies the elements of il into 
     * the container.
     */
    forward_list &operator =(std::initializer_list<value_type> il)
    {
        copy_from(il.begin(), il.end());
        return *this;
    }

    /**
     * Return iterator to before beginning
     * Returns an iterator pointing to the position before the first element 
     * in the container.
     */
    iterator before_begin() noexcept
    {
        return iterator(list_before_head(&lst_));
    }

    const_iterator before_begin() const noexcept
    {
        return const_iterator(list_before_head(&lst_));
    }

    /**
     * Return iterator to beginning
     * Returns an iterator pointing to the first element 
     * in the forward_list container.
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
     * in the forward_list container.
     */
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    /**
     * Return const_iterator to before beginning
     * Returns a const_iterator pointing to the position before the 
     * first element in the container.
     */
    const_iterator cbefore_begin() const noexcept
    {
        return const_iterator(list_before_head(&lst_));
    }

    /**
     * Return const_iterator to beginning
     * Returns a const_iterator pointing to the first element 
     * in the container.
     */
    const_iterator cbegin() const noexcept 
    {
        return const_iterator(list_head(&lst_)); 
    }

    /**
     * Return const_iterator to end
     * Returns a const_iterator pointing to the past-the-end element 
     * in the forward_list container.
     */
    const_iterator cend() const noexcept { return const_iterator(nullptr); }

    /**
     * Test whether array is empty
     * Returns a bool value indicating whether the forward_list container 
     * is empty, i.e. whether its size is 0.
     */
    bool empty() const noexcept { return list_is_empty(&lst_); }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }
    
    /**
     * Access first element
     * Returns a reference to the first element in the forward_list container.
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
     * Assign content
     * Assigns new contents to the container, replacing its current contents, 
     * and modifying its size accordingly.
     */

    /**
     * In the range version, the new contents are elements constructed 
     * from each of the elements in the range between first and last, 
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
    void assign(size_type n, const value_type &val)
    {
        fill(n, val);
    }

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
     * Inserts a new element at the beginning of the forward_list, 
     * right before its current first element. This new element is constructed 
     * in place using args as the arguments for its construction.
     */
    template <typename ... Args>
    void emplace_front(Args &&... args)
    {
        list_insert_after(list_before_head(&lst_), 
            create_node(std::forward<Args>(args) ...));
    }

    /**
     * Insert element at beginning
     * Inserts a new element at the beginning of the forward_list, 
     * right before its current first element. 
     * The content of val is copied (or moved) to the inserted element.
     */
    void push_front(const value_type &val)
    {
        list_insert_after(list_before_head(&lst_), create_node(val));
    }

    void push_front(value_type &&val)
    {
        list_insert_after(list_before_head(&lst_), 
            create_node(std::forward<value_type>(val)));
    }

    /**
     * Delete first element
     * Removes the first element in the forward_list container, 
     * effectively reducing its size by one.
     */
    void pop_front()
    {
        assert(!empty());
        destroy_node(list_delete_after(list_before_head(&lst_)));
    }

    /**
     * Construct and insert element
     * The container is extended by inserting a new element after the element 
     * at position. This new element is constructed in place using args 
     * as the arguments for its construction.
     */
    template <typename ... Args>
    iterator emplace_after(const_iterator position, Args &&... args)
    {
        link_type *new_link = create_node(std::forward<Args>(args) ...);
        list_insert_after((link_type *) position.link, new_link);
        return iterator(new_link);
    }

    /**
     * Insert elements
     * The container is extended by inserting new elements after 
     * the element at position.
     */
    iterator insert_after(const_iterator position, const value_type &val)
    {
        link_type *new_link = create_node(val);
        list_insert_after((link_type *) position.link, new_link);
        return iterator(new_link);
    }

    iterator insert_after(const_iterator position, value_type &&val)
    {
        link_type *new_link = create_node(std::forward<value_type>(val));
        list_insert_after((link_type *) position.link, new_link);
        return iterator(new_link);
    }

    iterator insert_after(const_iterator position, size_type n, 
        const value_type &val)
    {
        link_type *keep = (link_type *) position.link;
        for (size_type i = 0; i < n; ++i) {
            link_type *link = create_node(val);
            list_insert_after(keep, link);
            keep = keep->next;
        }
        return iterator(keep);
    }

    template <typename InputIterator, typename = typename
        std::enable_if<!std::is_integral<InputIterator>::value>::type>
    iterator insert_after(const_iterator position, InputIterator first, 
        InputIterator last)
    {
        link_type *keep = (link_type *) position.link;
        for ( ; first != last; ++first) {
            link_type *link = create_node(*first);
            list_insert_after(keep, link);
            keep = keep->next;
        }
        return iterator(keep);
    }

    iterator insert_after(const_iterator position, 
        std::initializer_list<value_type> il)
    {
        return insert_after(position, il.begin(), il.end());
    }

    /**
     * Erase elements
     * Removes from the forward_list container either a single element 
     * (the one after position) or a range of elements ((position,last))
     */
    iterator erase_after(const_iterator position)
    {
        destroy_node(list_delete_after((link_type *) position.link));
        return iterator((link_type *) position.link->next);
    }

    iterator erase_after(const_iterator position, const_iterator last)
    {
        auto link = (link_type *) position.link;
        auto nil = (link_type *) last.link;
        while (link->next != nil) {
            destroy_node(list_delete_after(link));
        }
        return iterator((link_type *) last.link);
    }

    /**
     * Swap content
     * Exchanges the content of the container by the content of fwdlst, 
     * which is another forward_list object of the same type. Sizes may differ.
     */
    void swap(forward_list& fwdlst)
    {
        swap_data(fwdlst);
        std::swap(node_alloc_, fwdlst.node_alloc_);
    }

    /**
     * Change size
     * Resizes the container to contain n elements.
     */
    void resize(size_type n)
    {
        resize(n, value_type());
    }

    void resize(size_type n, const value_type &val)
    {
        link_type *keep = &lst_.head;
        link_type *link = lst_.head.next;
        size_type i = 0;
        for ( ; link != nullptr && i < n; ++i) {
            keep = link;
            link = link->next;
        }
        if (link == nullptr) {
            for ( ; i < n; ++i) {
                list_insert_after(keep, create_node(val));
                keep = keep->next;
            }
        } else {
            while (keep->next != nullptr) {
                destroy_node(list_delete_after(keep));
            }
        }
    }

    /**
     * Clear content
     * Removes all elements from the forward_list container 
     * (which are destroyed), and leaving the container with a size of 0.
     */
    void clear() noexcept
    {
        finalize();
        initialize();
    }

    void splice_after(const_iterator position, forward_list &fwdlst)
    {
        splice_after((link_type *) position.link, 
            &fwdlst.lst_.head, nullptr);
    }

    void splice_after(const_iterator position, forward_list &&fwdlst)
    {
        splice_after((link_type *) position.link, 
            &fwdlst.lst_.head, nullptr);
    }

    void splice_after(const_iterator position, forward_list &fwdlst,
        const_iterator i)
    {
        splice_after((link_type *) position.link, 
            (link_type *) i.link, (link_type *) i.link->next->next);
    }

    void splice_after(const_iterator position, forward_list &&fwdlst,
        const_iterator i)
    {
        splice_after((link_type *) position.link, 
            (link_type *) i.link, (link_type *) i.link->next->next);
    }

    void splice_after(const_iterator position, forward_list &fwdlst,
        const_iterator first, const_iterator last)
    {
        splice_after((link_type *) position.link, 
            (link_type *) first.link, (link_type *) last.link);
    }

    void splice_after(const_iterator position, forward_list &&fwdlst,
        const_iterator first, const_iterator last)
    {
        splice_after((link_type *) position.link, 
            (link_type *) first.link, (link_type *) last.link);
    }

    void splice_after(link_type *position, link_type *before_head, link_type *nil)
    {
        if (before_head == nil || before_head->next == nil)
            return;

        link_type *head = before_head->next;
        link_type *tail = head;
        while (tail->next != nil) { // found tail
            tail = tail->next;
        }
        before_head->next = nil;    // delete (before_head, nil) from list, (before_head, nil) <==> [head, tail]
        tail->next = position->next;    // insert [head, tail] after position
        position->next = head;
    }

    /**
     * Remove elements with specific value
     * Removes from the container all the elements that compare equal to val. 
     * This calls the destructor of these objects and reduces 
     * the container size by the number of elements removed.
     */
    void remove(const value_type &val)
    {
        link_type *link = list_before_head(&lst_);
        while (link->next != nullptr) {
            auto node_val = static_cast<node_type *>(link->next)->valptr();
            if (*node_val == val) {
                auto node = list_delete_after(link);
                destroy_node(node);
            } else {
                link = link->next;
            }
        }
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
        link_type *link = list_before_head(&lst_);
        while (link->next != nullptr) {
            auto node_val = static_cast<node_type *>(link->next)->valptr();
            if (pred(*node_val)) {
                auto node = list_delete_after(link);
                destroy_node(node);
            } else {
                link = link->next;
            }
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
        while (pos != nullptr) {
            node_type *next = static_cast<node_type *>(pos->next);
            while (next != nullptr && binary_pred(*pos->valptr(), *next->valptr())) {
                list_delete_after(pos);
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
    void merge(forward_list &x)
    {
        merge(x, std::less<value_type>());
    }

    template <typename Compare>
    void merge(forward_list &x, Compare comp)
    {
        merge(x.lst_, comp);
    }

    void merge(forward_list &&x)
    {
        merge(std::move(x), std::less<value_type>());
    }

    template <typename Compare>
    void merge(forward_list &&x, Compare comp)
    {
        merge(x.lst_, comp);
    }

    template <typename Compare>
    void merge(list_type &lst, Compare comp)
    {
        link_type *dst_pos = list_before_head(&lst_);
        link_type *src_pos = list_before_head(&lst);
        while (src_pos->next != nullptr) {
            while (dst_pos->next != nullptr) {
                auto src_val = static_cast<node_type *>(src_pos->next)->valptr();
                auto dst_val = static_cast<node_type *>(dst_pos->next)->valptr();
                if (comp(*src_val, *dst_val)) { // src_val < dst_val
                    break;
                } else {    // dst_val >= src_val
                    dst_pos = dst_pos->next;
                }
            }
            auto node = list_delete_after(src_pos);
            list_insert_after(dst_pos, node);
            dst_pos = dst_pos->next;
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
        size_type n = list_size(&lst_);
        if (n < 2) return;

        link_type **array = new link_type *[n];

        link_type *link = list_head(&lst_);
        for (link_type **ptr = array; link != nullptr; link = link->next) {
            *ptr++ = link;
        }
        std::sort(array, array+n, make_compare(comp));

        initialize();
        link = list_before_head(&lst_);
        for (link_type **ptr = array, **end = array+n; ptr != end; link = link->next) {
            list_insert_after(link, *ptr++);
        }

        delete [] array;
    }
#else
    template <typename Compare>
    void sort(Compare comp)
    {
        link_type *sorted_last = list_before_head(&lst_);
        link_type *unsorted_first = sorted_last->next;
        while (unsorted_first != nullptr && unsorted_first->next != nullptr) {
            // get node of minimum value
            link_type *min_prev = sorted_last;
            link_type *min = unsorted_first;
            link_type *prev = unsorted_first;
            link_type *node = prev->next;
            while (node != nullptr) {
                if (comp(*static_cast<node_type *>(node)->valptr(), *static_cast<node_type *>(min)->valptr())) {
                    min = node;
                    min_prev = prev;
                }
                node = node->next;
                prev = prev->next;
            }

            // transplant node of minimum value to front
            if (min != unsorted_first) {
                list_delete_after(min_prev);
                list_insert_after(sorted_last, min);
            }
            sorted_last = sorted_last->next;
            unsorted_first = sorted_last->next;
        }
    }
#endif

    /**
     * Reverse the order of elements
     * Reverses the order of the elements in the forward_list container.
     */
    void reverse() noexcept
    {
        return list_reverse_after(list_before_head(&lst_));
    }

    /**
     * Get allocator
     * Returns a copy of the allocator object associated with the container.
     */
    allocator_type get_allocator() const noexcept
    {
        return allocator_type(node_alloc_);
    }

private:
    void swap_data(forward_list &fwdlst)
    {
        list_swap(&lst_, &fwdlst.lst_);
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

    struct destroy {
        destroy(node_alloc_type &alloc): node_alloc(&alloc) {}
    
        void operator ()(link_type *link)
        {
            node_type *node = static_cast<node_type *>(link);
            allocator_type(*node_alloc).destroy(node->valptr());
            node_alloc->deallocate(node, 1);
        }

        node_alloc_type *node_alloc;
    };

    void destroy_node(link_type *link)
    {
        destroy(node_alloc_).operator ()(link);
    }

    void range_destroy(link_type *first, link_type *last)
    {
//        list_foreach(first, last, destroy(node_alloc_));
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

    void initialize() { list_init(&lst_); }

    void finalize() 
    {
        while (!list_is_empty(&lst_)) {
            auto node = list_delete_head(&lst_);
            destroy_node(node);
        }
    }

    void fill(size_type n, const value_type &val)
    {
        link_type *keep = &lst_.head; 
        link_type *node = keep->next;
        while ((node != nullptr) && (n > 0)) {
            *static_cast<node_type *>(node)->valptr() = val;
            n--;
            keep = node;
            node = node->next;
        }

        if (node == nullptr) {    // forward_list.size() < n
            while (n > 0) {
                list_insert_after(keep, create_node(val));
                keep = keep->next;
                n--;
            }
        } else {    // forward_list.size() > n
            while (keep->next != nullptr) {
                destroy_node(list_delete_after(keep));
            }
        } 

        // forward_list.size() == n
    }

    template <typename InputIterator>
    void copy_from(InputIterator first, InputIterator last)
    {
        link_type *keep = &lst_.head;
        link_type *node = keep->next;
        while ((node != nullptr) && (first != last)) {
            *static_cast<node_type *>(node)->valptr() = *first;
            ++first;
            keep = node;
            node = node->next;
        }

        if (node == nullptr) {    // forward_list.size() < distance(first, last)
            while (first != last) {
                list_insert_after(keep, create_node(*first++));
                keep = keep->next;
            }
        } else {    // forward_list.size() > distance(first, last)
            while (keep->next != nullptr) {
                destroy_node(list_delete_after(keep));
            }
        } 
    
        // forward_list.size() == distance(first, last)
    }

    template <typename InputIterator>
    void move_from(InputIterator first, InputIterator last)
    {
        link_type *keep = &lst_.head;
        link_type *node = keep->next;
        while ((node != nullptr) && (first != last)) {
            *static_cast<node_type *>(node)->valptr() = std::move(*first);
            ++first;
            keep = node;
            node = node->next;
        }

        if (node == nullptr) {    // forward_list.size() < distance(first, last)
            while (first != last) {
                list_insert_after(keep, create_node(std::move(*first++)));
                keep = keep->next;
            }
        } else {    // forward_list.size() > distance(first, last)
            while (keep->next != nullptr) {
                destroy_node(list_delete_after(keep));
            }
        } 

        // forward_list.size() == distance(first, last)
    }
};

/**
 * Relational operators for forward_list
 * Performs the appropriate comparison operation between the forward_list 
 * containers lhs and rhs.
 */
template <typename T, typename Alloc>
inline
bool operator ==(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline
bool operator !=(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline
bool operator <(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline
bool operator >(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return (rhs < lhs);
}

template <typename T, typename Alloc>
inline
bool operator <=(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return !(lhs > rhs);
}

template <typename T, typename Alloc>
inline
bool operator >=(const forward_list<T, Alloc> &lhs, const forward_list<T, Alloc> &rhs)
{
    return !(lhs < rhs);
}

/**
 * Exchange contents of forward_lists
 * The contents of container x are exchanged with those of y. 
 * Both container objects must be of the same type (same template parameters), 
 * although sizes may differ.
 */
template <typename T, typename Alloc>
inline
void swap(forward_list<T, Alloc> &x, forward_list<T, Alloc> &y)
{
    return x.swap(y);
}

} // namespace mini_stl

#endif // HX_FORWARD_LIST_H

