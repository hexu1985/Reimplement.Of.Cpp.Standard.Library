/**
 * A unordered_map::iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct unordered_map_iterator {
	typedef singly_linked::list_t bucket_type;
	typedef singly_linked::list_node_t link_type;
	bucket_type* pos;
	bucket_type* end;
	link_type* link;

	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	typedef unordered_map_iterator<T> this_type;
	typedef unordered_map_node<T> node_type;

	unordered_map_iterator(): pos(nullptr), end(nullptr), link(nullptr) {}

	unordered_map_iterator(bucket_type* pos_, bucket_type* end_, 
		link_type* link_): pos(pos_), end(end_), link(link_) {} 
    
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
		this_type tmp(*this);
		next();
		return tmp;
	}

	bool operator==(const this_type& other) const
	{
		return (this->pos == other.pos) && (this->link == other.link);
	}

	bool operator!=(const this_type& other) const
	{
		return !(*this == other);
	}

	void next()
	{
		// get current bucket next
		link = link->next;

		// reach current bucket end
		if (link == nullptr) {
			assert(pos != end);	// if this is end, next is undefined
			// search next non-empty bucket
			for (pos += 1; pos != end && list_is_empty(pos); ++pos)
				;
			if (pos == end) {	// get bucket end
				link = nullptr;
			} else {	// get an non-empty bucket
				link = list_head(pos);
			}
		}
	}
};

/**
 * A unordered_map::const_iterator.
 * All the functions are op overloads.
 */
template <typename T>
struct unordered_map_const_iterator {
	typedef const singly_linked::list_t bucket_type;
	typedef const singly_linked::list_node_t link_type;
	bucket_type* pos;
	bucket_type* end;
	link_type* link;

	typedef T value_type;
	typedef const T *pointer;
	typedef const T &reference;
	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	typedef unordered_map_const_iterator<T> this_type;
	typedef const unordered_map_node<T> node_type;
	typedef unordered_map_iterator<T> iterator;

	unordered_map_const_iterator(): pos(nullptr), end(nullptr), link(nullptr) {}

	unordered_map_const_iterator(bucket_type* pos_, bucket_type* end_, 
		link_type* link_): pos(pos_), end(end_), link(link_) {}

	unordered_map_const_iterator(const iterator &iter): 
		pos(iter.pos), end(iter.end), link(iter.link) {}

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
		return (this->pos == other.pos) && (this->link == other.link);
	}

	bool operator!=(const this_type& other) const
	{
		return !(*this == other);
	}

	void next()
	{
		// get current bucket next
		link = link->next;

		// reach current bucket end
		if (link == nullptr) {
			assert(pos != end);	// if this is end, next is undefined
			// search next non-empty bucket
			for (pos += 1; pos != end && list_is_empty(pos); ++pos)
				;
			if (pos == end) {	// get bucket end
				link = nullptr;
			} else {	// get an non-empty bucket
				link = list_head(pos);
			}
		}
	}
};

/**
 * Unordered map iterator equality comparison.
 */
template <typename T>
inline 
bool operator==(const unordered_map_iterator<T>& x,
	const unordered_map_const_iterator<T>& y)
{
	return (x.pos == y.pos) && (x.link == y.link);
}

/**
 * Unordered map iterator inequality comparison.
 */
template <typename T>
inline 
bool operator!=(const unordered_map_iterator<T>& x,
	const unordered_map_const_iterator<T>& y)
{
	return !(x == y);
}


