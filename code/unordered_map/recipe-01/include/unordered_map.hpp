// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_UNORDERED_MAP_INC
#define MINI_STL_UNORDERED_MAP_INC

#include "singly_linked_list.hpp"
#include <memory>
#include <limits>
#include <functional>
#include <initializer_list>

namespace Hx {

/**
 * A helper node class for unordered_map.
 * This is just a linked list with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct unordered_map_node: public singly_linked::list_node_t {
    // raw storage buffer for type T
    // typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;     // deprecated in C++23
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

#include "unordered_map_local_iterator.hpp"
#include "unordered_map_iterator.hpp"

/**
 * Unordered Map
 * Unordered maps are associative containers that store elements formed by 
 * the combination of a key value and a mapped value, and which allows 
 * for fast retrieval of individual elements based on their keys.
 */
template <typename Key, typename T, 
    typename Hash = std::hash<Key>, 
    typename Pred = std::equal_to<Key>, 
    typename Alloc = std::allocator<std::pair<const Key, T>>> 
class unordered_map {
    typedef singly_linked::list_t bucket_type;
    typedef singly_linked::list_node_t link_type;
    typedef unordered_map_node<std::pair<const Key, T>> node_type;
    typedef typename Alloc::template rebind<bucket_type>::other bucket_alloc_type;
    typedef typename Alloc::template rebind<node_type>::other node_alloc_type;

    Hash hash_;                    // hash function
    Pred equal_;                // equal fucntion
    node_alloc_type node_alloc_;// allocator for node
    bucket_type* buckets_;        // hash table
    size_t bucket_count_;        // bucket count
    float max_load_factor_;        // max load factor

    static const size_t MIN_BUCKET_NUM_HINT = 8;
    static const size_t MAX_BUCKET_NUM_HINT = (((size_t)1 << (8*sizeof(size_t)-1)) / sizeof (bucket_type));
    static const size_t DEFAULT_BUCKET_NUM = 2*MIN_BUCKET_NUM_HINT-1;
    
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef Hash hasher;
    typedef Pred key_equal;
    typedef Alloc allocator_type;

    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef unordered_map_iterator<value_type> iterator;
    typedef unordered_map_const_iterator<value_type> const_iterator;
    typedef unordered_map_local_iterator<value_type> local_iterator;
    typedef unordered_map_const_local_iterator<value_type> const_local_iterator;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    /**
     * empty container constructor (default constructor)
     * Constructs an empty unordered_map object, containing no elements and 
     * with a size of zero.
     * It can construct the container with specific hasher, key_equal and 
     * allocator objects, along with a minimum number of hash buckets.
     */
    explicit unordered_map(size_type n = DEFAULT_BUCKET_NUM,
        const hasher& hf = hasher(),
        const key_equal& eql = key_equal(),
        const allocator_type& alloc = allocator_type()):
        hash_(hf), equal_(eql), node_alloc_(alloc), 
        buckets_(nullptr), bucket_count_(adjust_buckets(n)), 
        max_load_factor_(1.0)
    {
        initialize();
    }

    explicit unordered_map(const allocator_type& alloc): 
        hash_(hasher()), equal_(key_equal()), node_alloc_(alloc), 
        buckets_(nullptr), bucket_count_(DEFAULT_BUCKET_NUM), 
        max_load_factor_(1.0)
    {
        initialize();
    }

    /**
     * copy constructor
     * The object is initialized to have the same contents and properties 
     * as the ump unordered_map object.
     */
    unordered_map(const unordered_map& ump):
        hash_(ump.hash_), equal_(ump.equal_), node_alloc_(ump.node_alloc_),
        buckets_(nullptr), bucket_count_(ump.bucket_count_), 
        max_load_factor_(ump.max_load_factor_)
    {
        initialize();

        try 
        {
            copy_from(ump);
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    unordered_map(const unordered_map& ump, const allocator_type& alloc):
        hash_(ump.hash_), equal_(ump.equal_), node_alloc_(alloc),
        buckets_(nullptr), bucket_count_(ump.bucket_count_), 
        max_load_factor_(ump.max_load_factor_)
    {
        initialize();

        try 
        {
            copy_from(ump);
        }
        catch (...)
        {
            finalize();
            throw;
        }
    }

    /**
     * move constructor
     * The object acquires the contents of the rvalue ump.
     */
    unordered_map(unordered_map&& ump):
        hash_(std::move(ump.hash_)), equal_(std::move(ump.equal_)),
        node_alloc_(std::move(ump.node_alloc_)),
        buckets_(ump.buckets_), bucket_count_(ump.bucket_count_), 
        max_load_factor_(ump.max_load_factor_)
    {
        ump.buckets_ = nullptr;
        ump.bucket_count_ = DEFAULT_BUCKET_NUM;
        ump.max_load_factor_ = 1.0;
        ump.initialize();
    }

    unordered_map(unordered_map&& ump, const allocator_type& alloc):
        hash_(ump.hash_), equal_(ump.equal_), node_alloc_(alloc),
        buckets_(nullptr), bucket_count_(ump.bucket_count_), 
        max_load_factor_(ump.max_load_factor_)
    {
        initialize();

        try 
        {
            move_from(std::forward<unordered_map>(ump));
        }
        catch (...)
        {
            finalize();
            throw;
        }
        ump.clear();
    }

    /**
     * range constructor
     * Constructs an unordered_map object containing copies of each of 
     * the elements in the range [first,last).
     */
    template <typename InputIterator>
    unordered_map(InputIterator first, InputIterator last,
        size_type n = DEFAULT_BUCKET_NUM,
        const hasher& hf = hasher(),
        const key_equal& eql = key_equal(),
        const allocator_type& alloc = allocator_type()):
        hash_(hf), equal_(eql), node_alloc_(alloc), 
        buckets_(nullptr), bucket_count_(adjust_buckets(n)), 
        max_load_factor_(1.0)
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

    unordered_map(std::initializer_list<value_type> il,
        size_type n = DEFAULT_BUCKET_NUM,
        const hasher& hf = hasher(),
        const key_equal& eql = key_equal(),
        const allocator_type& alloc = allocator_type()):
        unordered_map(il.begin(), il.end(), n, hf, eql, alloc)
    {}

    /**
     * Destroy unordered map
     * Destructs the container object. This calls each of the contained 
     * element's destructors, and dealocates all the storage capacity 
     * allocated by the unordered_map container.
     */
    ~unordered_map() 
    {
        finalize(); 
    }

    /**
     * Assign content
     * Assigns ump (or il) as the new content for the container.
     */
    unordered_map& operator=(const unordered_map& ump)
    {
        if (this == &ump)
            return *this;

        unordered_map tmp(ump);
        this->swap_data(tmp);
        return *this;
    }

    unordered_map& operator=(unordered_map&& ump)
    {
        using std::swap;

        if (this == &ump)
            return *this;

        clear();
        swap_data(ump);
        swap(node_alloc_, ump.node_alloc_);
        return *this;
    }

    unordered_map& operator=(std::initializer_list<value_type> il)
    {
        clear();
        insert(il.begin(), il.end());
        return *this;
    }
    
    /**
     * Test whether container is empty
     * Returns a bool value indicating whether the unordered_map 
     * container is empty, i.e. whether its size is 0.
     */
    bool empty() const noexcept
    {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (!list_is_empty(buckets_+i))
                return false;
        }
        return true;
    }

    /**
     * Return container size
     * Returns the number of elements in the unordered_map container.
     */
    size_type size() const noexcept
    {
        size_type sum = 0;
        for (size_type i = 0; i < bucket_count_; ++i) {
            sum += bucket_size(i);
        }
        return sum;
    }

    /**
     * Return maximum size
     * Returns the maximum number of elements that the unordered_map 
     * container can hold.
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(node_type);
    }

    /**
     * Return iterator to beginning
     * Returns an iterator pointing to the first element in 
     * the unordered_map container or in one of its buckets.
     */
    iterator begin() noexcept
    {
        bucket_type* pos = buckets_;
        bucket_type* last = buckets_+bucket_count_;
        for ( ; pos != last && list_is_empty(pos); ++pos)
            ;
        if (pos == last)
            return end();
        else
            return iterator(pos, last, list_head(pos));
    }

    const_iterator begin() const noexcept 
    {
        const bucket_type* pos = buckets_;
        const bucket_type* last = buckets_+bucket_count_;
        for ( ; pos != last && list_is_empty(pos); ++pos)
            ;
        if (pos == last)
            return end();
        else
            return const_iterator(pos, last, list_head(pos));
    }

    local_iterator begin(size_type n)
    {
        assert(n < bucket_count());
        return local_iterator(list_head(buckets_+n));
    }

    const_local_iterator begin(size_type n) const
    {
        assert(n < bucket_count());
        return const_local_iterator(list_head(buckets_+n));
    }

    /**
     * Return iterator to end
     * Returns an iterator pointing to the past-the-end element in 
     * the unordered_map container or in one of its buckets.
     */
    iterator end() noexcept
    {
        return iterator(buckets_+bucket_count_, buckets_+bucket_count_, nullptr);
    }

    const_iterator end() const noexcept
    {
        return const_iterator(buckets_+bucket_count_, buckets_+bucket_count_, nullptr);
    }

    local_iterator end(size_type n)
    {
        assert(n < bucket_count());
        return local_iterator(nullptr);
    }

    const_local_iterator end(size_type n) const
    {
        assert(n < bucket_count());
        return const_local_iterator(nullptr);
    }

    /**
     * Return const_iterator to beginning
     * Returns a const_iterator pointing to the first element in 
     * the unordered_map container or in one of its buckets.
     */
    const_iterator cbegin() const noexcept 
    {
        const bucket_type* pos = buckets_;
        const bucket_type* last = buckets_+bucket_count_;
        for ( ; pos != last && list_is_empty(pos); ++pos)
            ;
        if (pos == last)
            return end();
        else
            return const_iterator(pos, last, list_head(pos));
    }

    const_local_iterator cbegin(size_type n) const
    {
        assert(n < bucket_count());
        return const_local_iterator(list_head(buckets_+n));
    }

    /**
     * Return const_iterator to end
     * Returns a const_iterator pointing to the past-the-end element in 
     * the unordered_map container or in one of its buckets.
     */
    const_iterator cend() const noexcept
    {
        return const_iterator(buckets_+bucket_count_, buckets_+bucket_count_, nullptr);
    }

    const_local_iterator cend(size_type n) const
    {
        assert(n < bucket_count());
        return const_local_iterator(nullptr);
    }

    /**
     * Access element
     * If k matches the key of an element in the container, 
     * the function returns a reference to its mapped value.
     */
    mapped_type& operator[](const key_type& k)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(k));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                return get_mapped(link->next);
            }
            link = link->next;
        }
        list_insert_after(link, create_node(k, mapped_type()));
        return get_mapped(link->next);
    }

    /**
     * Access element
     * Returns a reference to the mapped value of the element with key k 
     * in the unordered_map.
     */
    mapped_type& at(const key_type& k)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(k));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                return get_mapped(link->next);
            }
            link = link->next;
        }
        throw std::out_of_range("unordered_map::at");
    }

    const mapped_type& at(const key_type& k) const
    {
        const bucket_type* bucket = buckets_+bucket_index(hash_(k));
        const link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                return get_mapped(link->next);
            }
            link = link->next;
        }
        throw std::out_of_range("unordered_map::at");
    }

    /**
     * Get iterator to element
     * Searches the container for an element with k as key and returns 
     * an iterator to it if found, otherwise it returns an iterator to 
     * unordered_map::end (the element past the end of the container).
     */
    iterator find(const key_type& k)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(k));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                return iterator(bucket, buckets_+bucket_count_, link->next);
            }
            link = link->next;
        }
        return end();
    }

    const_iterator find(const key_type& k) const
    {
        const bucket_type* bucket = buckets_+bucket_index(hash_(k));
        const link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                return const_iterator(bucket, buckets_+bucket_count_, link->next);
            }
            link = link->next;
        }
        return cend();
    }

    /**
     * Count elements with a specific key
     * Searches the container for elements whose key is k and 
     * returns the number of elements found. 
     */
    size_type count(const key_type& k) const
    {
        const bucket_type* bucket = buckets_+bucket_index(hash_(k));
        const link_type* link = list_before_head(bucket);
        size_type n = 0;
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                n++;
            }
            link = link->next;
        }
        return n;
    }

    /**
     * Get range of elements with specific key
     * Returns the bounds of a range that includes all the elements 
     * in the container with a key that compares equal to k.
     */
    std::pair<iterator, iterator> equal_range(const key_type& k)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(k));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                iterator lower(bucket, buckets_+bucket_count_, link->next);
                iterator upper(lower);
                upper.next();
                return std::make_pair(lower, upper);
            }
            link = link->next;
        }
        return std::make_pair(end(), end());
    }

    std::pair<const_iterator, const_iterator> 
    equal_range(const key_type& k) const
    {
        const bucket_type* bucket = buckets_+bucket_index(hash_(k));
        const link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                const_iterator lower(bucket, buckets_+bucket_count_, link->next);
                const_iterator upper(lower);
                upper.next();
                return std::make_pair(lower, upper);
            }
            link = link->next;
        }
        return std::make_pair(cend(), cend());
    }

    /**
     * Construct and insert element
     * Inserts a new element in the unordered_map if its key is unique. 
     * This new element is constructed in place using args as the arguments 
     * for the element's constructor.
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        return insert(value_type(std::forward<Args>(args)...));
    }

    /**
     * Construct and insert element with hint
     * Inserts a new element in the unordered_map if its key is unique. 
     * This new element is constructed in place using args as the arguments 
     * for the element's constructor. position points to a location in the 
     * container suggested as a hint on where to start the search 
     * for its insertion point. 
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace_hint(const_iterator position, 
        Args&&... args)
    {
        return insert(position, value_type(std::forward<Args>(args)...));
    }

    /**
     * Insert elements
     * Inserts new elements in the unordered_map.
     */
    std::pair<iterator, bool> insert(const value_type& val)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(val.first));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), val.first)) {
                return std::make_pair(iterator(bucket, buckets_+bucket_count_, link->next), false);
            }
            link = link->next;
        }
        list_insert_after(link, create_node(val));
        return std::make_pair(iterator(bucket, buckets_+bucket_count_, link->next), true);
    }

    iterator insert(const_iterator hint, const value_type& val)
    {
        return insert(val).first;
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        for ( ; first != last; ++first) {
            const value_type& val = *first;
            insert(val);
        }
    }

    template <typename P>
    std::pair<iterator, bool> insert(P&& val)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(val.first));
        link_type* link = list_before_head(bucket);
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), val.first)) {
                return std::make_pair(iterator(bucket, buckets_+bucket_count_, link->next), false);
            }
            link = link->next;
        }
        list_insert_after(link, create_node(std::forward<P>(val)));
        return std::make_pair(iterator(bucket, buckets_+bucket_count_, link->next), true);
    }

    template <typename P>
    iterator insert(const_iterator hint, P&& val)
    {
        return insert(std::forward<P>(val)).first;
    }

    void insert(std::initializer_list<value_type> il)
    {
        insert(il.begin(), il.end());
    }

    /**
     * Erase elements
     * Removes from the unordered_map container a single element 
     */
    iterator erase(const_iterator position)
    {
        assert(position.pos != nullptr && position.link != nullptr);
        link_type* link = list_before_head(position.pos);
        while (link->next != nullptr) {
            if (link->next = position.link) {
                break;
            }
            link = link->next;
        }
        list_delete_after(link);
        destroy_node(position.pos);
        return iterator((bucket_type*) position.pos, (bucket_type*) position.end, link->next);
    }

    size_type erase(const key_type& k)
    {
        bucket_type* bucket = buckets_+bucket_index(hash_(k));
        link_type* link = list_before_head(bucket);
        size_t n = 0;
        while (link->next != nullptr) {
            if (equal_(get_key(link->next), k)) {
                link_type* node = link->next;
                list_delete_after(link);
                destroy_node(node);
                n++;
            }
            link = link->next;
        }
        return n;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        while (first != last) {
            first = erase(first);
        }
        return iterator((bucket_type*) last.pos, (bucket_type*) last.end, (link_type*) last.link);
    }

    /**
     * Clear content
     * All the elements in the unordered_map container are dropped: 
     * their destructors are called, and they are removed from the container, 
     * leaving it with a size of 0.
     */
    void clear() noexcept
    {
        bucket_type* bucket = buckets_;
        bucket_type* last = buckets_+bucket_count_;
        for ( ; bucket != last; ++bucket) {
            while (!list_is_empty(bucket)) {
                auto node = list_delete_head(bucket);
                destroy_node(node);
            }
        }
    }

    /**
     * Swap content
     * Exchanges the content of the container by the content of ump, 
     * which is another unordered_map object containing elements 
     * of the same type. Sizes may differ.
     */
    void swap(unordered_map &ump)
    {
        using std::swap;

        swap_data(ump);
        swap(node_alloc_, ump.node_alloc_);
    }

    /**
     * Return number of buckets
     * Returns the number of buckets in the unordered_map container.
     */
    size_type bucket_count() const noexcept
    {
        return bucket_count_;
    }

    /**
     * Return maximum number of buckets
     * Returns the maximum number of buckets that the unordered_map 
     * container can have.
     */
    size_type max_bucket_count() const noexcept
    {
        return MAX_BUCKET_NUM_HINT-1;
    }

    /**
     * Return bucket size
     * Returns the number of elements in bucket n.
     */
    size_type bucket_size(size_type n) const
    {
        assert(n < bucket_count());
        return list_size(buckets_+n);
    }

    /**
     * Locate element's bucket
     * Returns the bucket number where the element with key k is located.
     */
    size_type bucket(const key_type &k) const
    {
        return bucket_size(bucket_index(hash_(k)));
    }

    /**
     * Return load factor
     * Returns the current load factor in the unordered_map container.
     */
    float load_factor() const noexcept
    {
        return (float) size() / bucket_count();
    }

    /**
     * Get maximum load factor
     * returns the current maximum load factor for the unordered_map container.
     */
    float max_load_factor() const noexcept
    {
        return max_load_factor_;
    }

    /**
     * Set maximum load factor
     * sets z as the new maximum load factor for the unordered_map container.
     */
    void max_load_factor(float z)
    {
        max_load_factor_ = z;
    }

    /**
     * Set number of buckets
     * Sets the number of buckets in the container to n or more.
     */
    void rehash(size_type n)
    {
        size_type buckets_num =
            adjust_buckets(std::max(n, size()/max_load_factor()));
        do_rehash(buckets_num);
    }

    /**
     * Request a capacity change
     * Sets the number of buckets in the container (bucket_count)
     * to the most appropriate to contain at least n elements.
     */
    void reserve(size_type n)
    {
        size_type buckets_num = 
            adjust_buckets(std::max(n, size())/max_load_factor());
        do_rehash(buckets_num);
    }

    /**
     * Shrink to specified bucket count
     * Requests the container to reduce its buckets to specified bucket count.
     */
    void shrink(size_type n)
    {
        if (n >= size())
            return;
        do_rehash(adjust_buckets(n));
    }

    /**
     * Shrink to fit
     * Requests the container to reduce its buckets to fit its size.
     */
    void shrink_to_fit()
    {
        shrink(adjust_buckets(size()/max_load_factor()));
    }

    /**
     * Get hash function
     * Returns the hash function object used by the hash_map container.
     */
    hasher hash_function() const
    {
        return hash_;
    }

    /**
     * Get key equivalence predicate
     * Returns the key equivalence comparison predicate used
     * by the hash_map container.
     */
    key_equal key_eq() const
    {
        return equal_;
    }

    /**
     * Get allocator
     * Returns the allocator object used to construct the container.
     */
    allocator_type get_allocator() const
    {
        return allocator_type(node_alloc_);
    }

    /**
     * Relational operators for unordered_map
     * These overloaded global operator functions perform the appropriate 
     * equality or inequality comparison operation between the unordered_map 
     * containers lhs and rhs.
     */
    bool operator==(const unordered_map& other) const
    {
        return (size() == other.size() && 
            contain(other) && other.contain(*this));
    }

    bool operator!=(const unordered_map& other) const
    {
        return !(this->operator ==(other));
    }

private:
    static size_type adjust_buckets(size_type buckets_num)
    {
        if (buckets_num > MAX_BUCKET_NUM_HINT) {
            return MAX_BUCKET_NUM_HINT-1;
        }

        size_type n = 2*MIN_BUCKET_NUM_HINT;
        while (n-1 < buckets_num) {
            n = n << 1;
        }
        return n-1;
    }

    static size_type bucket_index(size_type hash_val, size_type buckets_num)
    {
        return hash_val % buckets_num;
    }

    size_type bucket_index(size_type hash_val) const
    {
        return bucket_index(hash_val, bucket_count_);
    }

    void copy_from(const unordered_map& ump)
    {
        bucket_type* first = ump.buckets_; 
        bucket_type* last = ump.buckets_+ump.bucket_count_;

        for (bucket_type* pos = first; pos != last; ++pos) {
            if (list_is_empty(pos))
                continue;
            bucket_type* bucket = buckets_+(pos-first);
            link_type* link = list_head(pos);
            link_type* dest_link = list_before_head(bucket);
            while (link != nullptr) {
                node_type* node = static_cast<node_type*>(link);
                list_insert_after(dest_link, create_node(*node->valptr()));
                link = link->next;
                dest_link = dest_link->next;
            }
        }
    }

    void move_from(unordered_map&& ump)
    {
        bucket_type* first = ump.buckets_; 
        bucket_type* last = ump.buckets_+ump.bucket_count_;

        for (bucket_type* pos = first; pos != last; ++pos) {
            if (list_is_empty(pos))
                continue;
            bucket_type* bucket = buckets_+(pos-first);
            link_type* link = list_head(pos);
            link_type* dest_link = list_before_head(bucket);
            while (link != nullptr) {
                node_type* node = static_cast<node_type*>(link);
                list_insert_after(dest_link, create_node(std::move(*node->valptr())));
                link = link->next;
                dest_link = dest_link->next;
            }
        }
    }

    void do_rehash(size_type n)
    {
        if (n == bucket_count_) return;

        // allocate new table 
        bucket_type* new_buckets = bucket_alloc_type(node_alloc_).allocate(n);
        for (size_type i = 0; i < n; ++i)
            list_init(new_buckets+i);

        // rehash and transfer node to new table
        bucket_type* bucket = buckets_;
        bucket_type* last = buckets_+bucket_count_;
        for ( ; bucket != last; ++bucket) {
            while (!list_is_empty(bucket)) {
                auto link = list_delete_head(bucket);
                bucket_type* new_bucket = new_buckets+bucket_index(hash_(get_key(link)), n);
                list_insert_after(list_before_head(new_bucket), link);
            }
        }
        
        // deallocate old table
        bucket_alloc_type(node_alloc_).deallocate(buckets_, bucket_count_);
        buckets_ = new_buckets;
        bucket_count_ = n;
    }

    void swap_data(unordered_map &ump)
    {
        std::swap(buckets_, ump.buckets_);
        std::swap(bucket_count_, ump.bucket_count_);
        std::swap(max_load_factor_, ump.max_load_factor_);
    }

    bool contain(const unordered_map &ump) const
    {
        const_iterator pos = ump.begin();
        const_iterator last = ump.end(); 
        const_iterator this_last = this->end();
        for ( ; pos != last; ++pos) {
            const_iterator this_pos = this->find(pos->first);
            if (this_pos == this_last || this_pos->second != pos->second)
                return false;
        }
        return true;
    }

    node_type* get_node()
    {
        return node_alloc_.allocate(1);
    }

    void put_node(node_type* node)
    {
        node_alloc_.deallocate(node, 1);
    }

    template <typename... Args>
    link_type* create_node(Args&&... args)
    {
        node_type* node = get_node();
        try
        {
            allocator_type(node_alloc_).construct(
                node->valptr(), std::forward<Args>(args)...);
        }
        catch (...)
        {
            put_node(node);
            throw;
        }
        return static_cast<link_type*>(node);
    }

    struct destroy {
        node_alloc_type* node_alloc;

        destroy(node_alloc_type &node_alloc_): node_alloc(&node_alloc_) {}
    
        void operator ()(link_type* link)
        {
            node_type* node = static_cast<node_type*>(link);
            allocator_type(*node_alloc).destroy(node->valptr());
            node_alloc->deallocate(node, 1);
        }
    };

    void destroy_node(link_type* link)
    {
        destroy(node_alloc_).operator ()(link);
    }

    static const key_type& get_key(const link_type* link)
    {
        const node_type* node = static_cast<const node_type*>(link);
        return node->valptr()->first;
    }

    static mapped_type& get_mapped(link_type* link)
    {
        node_type* node = static_cast<node_type*>(link);
        return node->valptr()->second;
    }

    static const mapped_type& get_mapped(const link_type* link)
    {
        const node_type* node = static_cast<const node_type*>(link);
        return node->valptr()->second;
    }

    void initialize()
    {
        buckets_ = bucket_alloc_type(node_alloc_).allocate(bucket_count_);
        for (size_type i = 0; i < bucket_count_; ++i)
            list_init(buckets_+i);
    }

    void finalize()
    {
        clear();
        bucket_alloc_type(node_alloc_).deallocate(buckets_, bucket_count_);
    }
};
} // namespace Hx

#endif  // MINI_STL_UNORDERED_MAP_INC
