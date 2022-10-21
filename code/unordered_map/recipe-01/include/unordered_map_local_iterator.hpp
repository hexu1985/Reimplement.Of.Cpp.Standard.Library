/**
 * A unordered_map::local_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct unordered_map_local_iterator {
    typedef singly_linked::list_node_t link_type;
    link_type* link;

	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	typedef unordered_map_local_iterator<T> this_type;
	typedef unordered_map_node<T> node_type;

	unordered_map_local_iterator(): link(nullptr) {}

	explicit unordered_map_local_iterator(link_type* link_): link(link_) {}

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
        assert(link != nullptr);
		return !(*this == other);
	}

	void next()
	{
		link = link->next;
	}
};

/**
 * A unordered_map::const_local_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct unordered_map_const_local_iterator {
    typedef const singly_linked::list_node_t link_type;
    link_type* link;

	typedef T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	typedef unordered_map_const_local_iterator<T> this_type;
	typedef const unordered_map_node<T> node_type;
	typedef unordered_map_local_iterator<T> local_iterator;

	unordered_map_const_local_iterator(): link(nullptr) {}

	explicit unordered_map_const_local_iterator(link_type* link_): link(link_) {}

	unordered_map_const_local_iterator(const local_iterator& iter): link(iter.link) {}

	reference operator*() const
	{
		return *static_cast<node_type*>(link)->valptr();
	}

	pointer operator->() const
	{
		return static_cast<node_type*>(link)->valptr();
	}

	this_type& operator++()
	{
		next();
		return *this;
	}

	this_type operator++(int)
	{
		this_type temp(*this);
		next();
		return temp;
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
		link = link->next;
	}
};

/**
 * Unordered map local iterator equality comparison.
 */
template <typename T>
inline 
bool operator==(const unordered_map_local_iterator<T>& x,
	const unordered_map_const_local_iterator<T>& y)
{
	return (x.link == y.link);
}

/**
 * Unordered map local iterator inequality comparison.
 */
template <typename T>
inline 
bool operator!=(const unordered_map_local_iterator<T>& x,
	const unordered_map_const_local_iterator<T>& y)
{
	return !(x == y);
}


