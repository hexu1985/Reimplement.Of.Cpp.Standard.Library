/**
 * @file vector.hpp
 * @brief 封装动态数组的顺序容器
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-11-03
 */

#ifndef MINI_STL_VECTOR_INC
#define MINI_STL_VECTOR_INC

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <limits>
#include <algorithm>
#include <initializer_list>
#include <type_traits>

namespace mini_stl {

/**
 * @brief 封装动态数组的顺序容器
 *
 * @tparam T 元素的类型
 * @tparam Alloc 用于获取/释放内存及构造/析构内存中元素的分配器。类型必须满足分配器 (Allocator) 的要求。
 */
template <typename T, typename Alloc = std::allocator<T> >
class vector {
public:
	/* Type Definitions of Vectors */
	typedef	T value_type;
	typedef Alloc allocator_type;

	typedef T &reference;
	typedef const T &const_reference;
	typedef typename std::allocator_traits<allocator_type>::pointer pointer;
	typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef typename std::iterator_traits<iterator>::difference_type difference_type;
	typedef size_t size_type;

private:
	// member data
	allocator_type alloc_;
	pointer start_;				// data begin position
	pointer finish_;			// data end postion
	pointer end_of_storage_;	// storage end postion

public:
    /**
     * @brief 默认构造函数。构造拥有默认构造的分配器的空容器。
     */
	explicit vector():
        vector(allocator_type())
	{
	} 

    /**
     * @brief 构造拥有给定分配器 alloc 的空容器。
     *
     * @param alloc 用于此容器所有内存分配的分配器
     */
	explicit vector(const allocator_type &alloc):
		alloc_(alloc) 
	{
		initialize();
	} 

    /**
     * @brief 构造拥有个 n 默认插入的 T 实例的容器。不进行复制。
     *
     * @param n 容器的大小
     */
	explicit vector(size_type n): vector(n, value_type(), allocator_type()) {}

    /**
     * @brief  构造拥有 count 个有值 val 的元素的容器。
     *
     * @param n 容器的大小
     * @param val 以之初始化容器元素的值
     * @param alloc 用于此容器所有内存分配的分配器
     */
	vector(size_type n, const value_type &val, 
		const allocator_type &alloc = allocator_type()): alloc_(alloc)
	{
		if (n == 0) {
			initialize();
			return;
		}

		initialize(n);

		try
		{
			finish_ = std::uninitialized_fill_n(start_, n, val);
		}
		catch (...)
		{
			finalize();
			throw;
		}
	}

    /**
     * @brief 构造拥有范围 [first, last) 内容的容器。
     *
     * @param first, last 复制元素的来源范围
     * @param alloc 用于此容器所有内存分配的分配器
     */
	template <typename InputIterator, typename = typename
		std::enable_if<!std::is_integral<InputIterator>::value>::type>
	vector(InputIterator first, InputIterator last,
		const allocator_type &alloc = allocator_type()): alloc_(alloc)
	{
		size_type n = std::distance(first, last);
		if (n == 0) {
			initialize();
			return;
		}

		initialize(n);

		try
		{
			finish_ = std::uninitialized_copy(first, last, start_);
		}
		catch (...)
		{
			finalize();
			throw;
		}
	}
	
    /**
     * @brief 复制构造函数。构造拥有 x 内容的容器
     *
     * @param x 用作初始化容器元素来源的另一容器
     */
	vector(const vector &x): alloc_(x.alloc_) 
	{
		size_type n = x.size();
		if (n == 0) {
			initialize();
			return;
		}

		initialize(n);

		try
		{
			finish_ = std::uninitialized_copy(x.begin(), x.end(), start_);
		}
		catch (...)
		{
			finalize();
			throw;
		}
	}

    /**
     * @brief 构造拥有 x 内容的容器，以 alloc 为分配器。
     *
     * @param x 用作初始化容器元素来源的另一容器
     * @param alloc 用于此容器所有内存分配的分配器
     */
	vector(const vector &x, const allocator_type &alloc): alloc_(alloc)
	{
		size_type n = x.size();
		if (n == 0) {
			initialize();
			return;
		}

		initialize(n);

		try
		{
			finish_ = std::uninitialized_copy(x.begin(), x.end(), start_);
		}
		catch (...)
		{
			finalize();
			throw;
		}
	}

    /**
     * @brief 移动构造函数。用移动语义构造拥有 x 内容的容器。
     * 分配器通过属于 x 的分配器移动构造获得。移动后，保证 x 为 empty() 。
     *
     * @param x 用作初始化容器元素来源的另一容器
     */
	vector(vector &&x): 
		alloc_(std::move(x.alloc_)),
		start_(x.start_), finish_(x.finish_), end_of_storage_(x.end_of_storage_)
	{
		x.initialize();
	}

    /**
     * @brief 有分配器扩展的移动构造函数。以 alloc 为新容器的分配器，从 x 移动内容；
     *        alloc != x.get_allocator() ，则它导致逐元素移动。（该情况下，移动后不保证 x 为空）
     *
     * @param x 用作初始化容器元素来源的另一容器
     * @param alloc 用于此容器所有内存分配的分配器
     */
	vector(vector &&x, const allocator_type &alloc): alloc_(alloc)
	{
		size_type n = x.size();
		if (n == 0) {
			initialize();
			return;
		}

		initialize(n);

		try
		{
			finish_ = std::uninitialized_move(x.begin(), x.end(), start_);
		}
		catch (...)
		{
			finalize();
			throw;
		}
	}

    /**
     * @brief 构造拥有 initializer_list il 内容的容器。
     *
     * @param il 用作初始化元素来源的 initializer_list
     * @param alloc 用于此容器所有内存分配的分配器
     */
	vector(std::initializer_list<value_type> il,
		const allocator_type &alloc = allocator_type()): 
		vector(il.begin(), il.end(), alloc) {}

    /**
     * @brief 销毁 vector 。调用元素的析构函数，然后解分配所用的存储。注意，若元素是指针，则不销毁所指向的对象。
     */
	~vector() { finalize(); }

    /**
     * @brief 复制赋值运算符。以 x 的副本替换内容。
     *
     * @param x 用作数据源的另一容器
     *
     * @return *this
     */
	vector &operator =(const vector &x)
	{
		if (this == &x)
			return *this;

		assign(x.begin(), x.end());
		return *this;
	}

    /**
     * @brief 移动赋值运算符。用移动语义以 x 的内容替换内容（即从 x 移动 x 中的数据到此容器）。
     *        之后 x 在合法但未指定的状态。
     *
     * @param x 用作数据源的另一容器
     *
     * @return *this
     */
	vector &operator =(vector &&x)
	{
		if (this == &x)
			return *this;

		this->clear();
		this->swap_data(x);
		return *this;
	}

    /**
     * @brief 以 initializer_list il 所标识者替换内容。
     *
     * @param il 用作数据源的 initializer_list
     *
     * @return *this
     */
	vector &operator =(std::initializer_list<value_type> il)
	{
		assign(il.begin(), il.end());
		return *this;
	}

    /**
     * @brief 返回指向 vector 首元素的迭代器。
     *        若 vector 为空，则返回的迭代器将等于 end() 。
     *
     * @return 指向首元素的迭代器。
     */
	iterator begin() noexcept { return start_; }
	const_iterator begin() const noexcept { return start_; }
	const_iterator cbegin() const noexcept { return start_; }

    /**
     * @brief 返回指向 vector 末元素后一元素的迭代器。
     *        此元素表现为占位符；试图访问它导致未定义行为。
     *
     * @return 指向后随最后元素的迭代器。 
     */
	iterator end() noexcept { return finish_; }
	const_iterator end() const noexcept { return finish_; }
	const_iterator cend() const noexcept { return finish_; }

    /**
     * @brief 返回指向逆向 vector 首元素的逆向迭代器。它对应非逆向 vector 的末元素。若 vector 为空，则返回的迭代器等于 rend() 。
     *
     * @return 指向首元素的逆向迭代器。
     */
	reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    /**
     * @brief 返回指向逆向 vector 末元素后一元素的逆向迭代器。它对应非逆向 vector 首元素的前一元素。此元素表现为占位符，试图访问它导致未定义行为。
     *
     * @return 指向末元素后一元素的逆向迭代器。
     */
	reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    /**
     * @brief 返回容器中的元素数，即 std::distance(begin(), end()) 。
     *
     * @return 容器中的元素数量。
     */
	size_type size() const noexcept { return (finish_-start_); }

	/**
	 * Return maximum size
	 * Returns the maximum number of elements that the vector can hold.
	 */
	size_type max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
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
		if (n > capacity()) {
			reserve(adjust_capacity(n));
		}

		size_type sz = size();
		if (sz < n) {	// this->size() < n <= capacity()
            std::uninitialized_fill(finish_, start_+n, val);
		} else {	// n < this->size()
			range_destroy(start_+n, finish_);
		} 

		// update this size
		finish_ = start_+n;
	}

	/**
	 * Return size of allocated storage capacity
	 * Returns the size of the storage space currently allocated 
	 * for the vector, expressed in terms of elements.
	 */
	size_type capacity() const noexcept { return (end_of_storage_-start_); }

	/**
	 * Test whether vector is empty
	 * Returns whether the vector is empty (i.e. whether its size is 0).
	 * This function does not modify the container in any way. 
	 */
	bool empty() const { return (finish_ == start_); }

	/**
	 * Test whether vector is full
	 * Returns whether the vector is full 
	 * (i.e. whether its size is its capacity).
	 * This function does not modify the container in any way. 
	 */
	bool full() const { return (finish_ == end_of_storage_); }

	/**
	 * Request a change in capacity
	 * Requests that the vector capacity be at least enough 
	 * to contain n elements.
	 */
	void reserve(size_type n)
	{
		if (n <= capacity()) return;

		// allocate newbuf and remember oldbuf, oldbuf's size and capacity
		pointer new_start = allocate(n);
		pointer new_finish = NULL;

		// copy from oldbuf to newbuf and set newbuf's size and capacity
		try
		{
			new_finish = std::uninitialized_move(start_, finish_, new_start);
		}
		catch (...)
		{
			deallocate(new_start, n);
			throw;
		}
		finalize();
		start_ = new_start;
		finish_ = new_finish;
		end_of_storage_ = new_start+n;
	}

	/**
	 * Shrink to fit
	 * Requests the container to reduce its capacity to fit its size.
	 */
	void shrink_to_fit()
	{
		if (size() > capacity()/2)
			return;

		vector x(std::move(*this), this->alloc_);
		this->swap_data(x);
	}

	/**
	 * Access element
	 * Returns a reference to the element at position n in the vector container.
	 */
	reference operator [](size_type n) { return *(start_+n); }
	const_reference operator [](size_type n) const { return *(start_+n); }
	
	/**
	 * Access element
	 * Returns a reference to the element at position n in the vector.
	 */
	reference at(size_type n)
	{
		if (n >= size())
			throw std::out_of_range(__func__);
		return *(start_+n); 
	}

	const_reference at(size_type n) const
	{
		if (n >= size())
			throw std::out_of_range(__func__);
		return *(start_+n); 
	}
	
	/**
	 * Access first element
	 * Returns a reference to the first element in the vector.
	 */
	reference front() 
	{
		assert(!empty());
		return *start_;
	}

	const_reference front() const 
	{
		assert(!empty());
		return *start_;
	}

	/**
	 * Access last element
	 * Returns a reference to the last element in the vector.
	 */
	reference back() 
	{
		assert(!empty());
		return *(finish_-1); 
	}

	const_reference back() const 
	{
		assert(!empty());
		return *(finish_-1);
	}

	/**
	 * Access data
	 * Returns a direct pointer to the memory array used internally by 
	 * the vector to store its owned elements.
	 */
	value_type *data() noexcept { return start_; }
	const value_type *data() const noexcept { return start_; }

	/**
	 * Assign vector content
	 * Assigns new contents to the vector, replacing its current contents, 
	 * and modifying its size accordingly.
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
		size_type n = std::distance(first, last);
		if (n > capacity()) {
			vector x(first, last, alloc_);
			this->swap_data(x);
			return;
		}

		// assign elements
		size_type sz = size();
		std::copy_n(first, std::min(n, sz), start_);

		if (sz < n) {	// this->size() < std::distance(first, last)
			std::advance(first, sz);
            std::uninitialized_copy(first, last, finish_);
		} else {	// this->size() >= std::distance(first, last)
			range_destroy(start_+n, finish_);
		}

		// update this size
		finish_ = start_+n;
	}

	/**
	 * In the fill version, the new contents are n elements, 
	 * each initialized to a copy of val.
	 */
	void assign(size_type n, const value_type &val)
	{
		if (n > capacity()) {
			vector x(n, val, alloc_);
			this->swap_data(x);
			return;
		}

		// assign elements
		size_type sz = size();
		std::fill_n(start_, std::min(n, sz), val);

		if (sz < n) {	// this->size() < std::distance(first, last)
            std::uninitialized_fill(start_+sz, start_+n, val);
		} else if (sz > n) {	// this->size() > std::distance(first, last)
			range_destroy(start_+n, finish_);
		}

		// update this size
		finish_ = start_+n;
	}

	/**
	 * In the initializer list version, 
	 * the new contents are copies of the values passed as initializer list, 
	 * in the same order.
	 */
	void assign(std::initializer_list<value_type> il)
	{
		assign(il.begin(), il.end());
	}

	/**
	 * Add element at the end
	 * Adds a new element at the end of the vector, after its current last 
	 * element. The content of val is copied (or moved) to the new element.
	 */
	void push_back(const value_type &val)
	{
		if (full()) {
			reserve(adjust_capacity());
		}

		construct(finish_++, val);
	}

	void push_back(value_type &&val)
	{
		if (full()) {
			reserve(adjust_capacity());
		}

		construct(finish_++, std::forward<value_type>(val));
	}

	/**
	 * Delete last element
	 * Removes the last element in the vector, effectively reducing 
	 * the container size by one.
	 */
	void pop_back()
	{
		assert(!empty());
		destroy(finish_--);
	}

	/**
	 * Insert elements
	 * The vector is extended by inserting new elements before the element 
	 * at the specified position, effectively increasing the container size 
	 * by the number of elements inserted.
	 */
	iterator insert(const_iterator position, const value_type &val)
	{
		return insert(position, 1, val);
	}

	iterator insert(const_iterator position, size_type n, const value_type &val)
	{
		assert(position <= finish_); 

		if (n == 0)
			return (iterator) position;

		if (capacity() < size()+n) {	// need reallocate
			size_t idx = position-start_;
			reserve(adjust_capacity(size()+n));
			position = start_+idx;
		}

		pointer pos = (pointer) position;
		if (pos == finish_ || vacate(pos, n)) {
            std::uninitialized_fill_n(pos, n, val);
		} else {
			std::fill_n(pos, n, val);
		}
		finish_ += n;
		return (iterator) pos;
	}

	template <typename InputIterator, typename = typename
		std::enable_if<!std::is_integral<InputIterator>::value>::type>
	iterator insert(const_iterator position, 
		InputIterator first, InputIterator last)
	{
		assert(position <= finish_); 

		size_t n = std::distance(first, last);

		if (n == 0)
			return (iterator) position;

		if (capacity() < size()+n) {	// need reallocate
			size_t idx = position-start_;
			reserve(size()+n);
			position = start_+idx;
		}

		pointer pos = (pointer) position;
		if (pos == finish_ || vacate(pos, n)) {
            std::uninitialized_copy(first, last, pos);
		} else {
			std::copy(first, last, pos);
		}
		finish_ += n;
		return (iterator) pos;
	}
	
	iterator insert(const_iterator position, value_type &&val)
	{
		if (full()) {	// need reallocate
			size_t idx = position-start_;
			reserve(adjust_capacity());
			position = start_+idx;
		}

		pointer pos = (pointer) position;
		if (pos == finish_ || vacate(pos, 1)) {
			construct(pos, std::move(val));
		} else {
			*pos = std::move(val);
		}
		finish_++;
		return (iterator) pos;
	}

	iterator insert(const_iterator position, 
		std::initializer_list<value_type> il)
	{
		return insert(position, il.begin(), il.end());
	}

	/**
	 * Erase elements
	 * Removes from the vector either a single element (position) 
	 * or a range of elements ([first,last)).
	 */
	iterator erase(const_iterator position)
	{
		assert(!empty() && position <= end());

		if (position == end()) return end();

		std::move((iterator) position+1, finish_, (iterator) position);
		destroy(--finish_);
		return (iterator) position;
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		assert(!empty() && first <= last);

		if (first == last) 
			return (iterator) first;

		iterator iter = std::move((iterator) last, finish_, (iterator) first);
		range_destroy(iter, finish_);
		finish_ = iter;
		return (iterator) first;
	}

	/**
	 * Swap content
	 * Exchanges the content of the container by the content of x, 
	 * which is another vector object of the same type. Sizes may differ.
	 */
	void swap(vector &x)
	{
		using std::swap;

		this->swap_data(x);
		swap(alloc_, x.alloc_);
	}

	/**
	 * Clear content
	 * Removes all elements from the vector (which are destroyed), 
	 * leaving the container with a size of 0.
	 */
	void clear() noexcept
	{
		range_destroy(start_, finish_);
		finish_ = start_;
	}

	/**
	 * Construct and insert element
	 * The container is extended by inserting a new element at position. 
	 * This new element is constructed in place using args as the arguments 
	 * for its construction.
	 */
	template <typename ... Args>
	iterator emplace(const_iterator position, Args &&... args)
	{
		if (full()) {	// need reallocate
			size_t off = position-start_;
			reserve(adjust_capacity());
			position = start_+off;
		}

		pointer pos = (pointer) position;
		if (pos == finish_ || vacate(pos, 1)) {
			construct(finish_, std::forward<Args>(args) ...);
		} else {
			*pos = value_type(std::forward<Args>(args) ...);
		}
		finish_++;
		return (iterator) pos;
	}

	/**
	 * Construct and insert element at the end
	 * Inserts a new element at the end of the vector, right after its 
	 * current last element. This new element is constructed in place 
	 * using args as the arguments for its constructoe.
	 */
	template <typename ... Args>
	void emplace_back(Args &&... args)
	{
		if (full()) {
			reserve(adjust_capacity());
		}

		construct(finish_++, std::forward<Args>(args) ...);
	}

	/**
	 * Get allocator
	 * Returns a copy of the allocator object associated with the vector.
	 */
	allocator_type get_allocator() const noexcept
	{
		return alloc_;
	}

private:
	size_type adjust_capacity(size_type hint = 0)
	{
		if (hint < 4) hint = 4;
		return std::max(hint, 2*capacity());
	}

	void swap_data(vector &x)
	{
		std::swap(start_, x.start_);
		std::swap(finish_, x.finish_);
		std::swap(end_of_storage_, x.end_of_storage_);
	}

	/**
	 * vacate n element's space for insert before pos.
	 * if uninitialized buffer, return true, otherwise return false
	 */
	bool vacate(pointer pos, size_type n)
	{
		assert(pos < finish_ && finish_+n <= end_of_storage_);
		if (finish_-pos <= n) {
            std::uninitialized_move(pos, finish_, pos+n);
			range_destroy(pos, finish_);
			return true;
		} else {	// finish_-pos > n
            std::uninitialized_move_n(finish_-n, n, finish_);
			std::move_backward(pos, finish_-n, finish_);
			return false;
		}
	}

	pointer allocate(size_type n)
	{
		if (n == 0)
			return NULL;

		return std::allocator_traits<allocator_type>::allocate(alloc_, n);
	}

	void deallocate(pointer p, size_type n)
	{
		if (p == NULL || n == 0)
			return;

		std::allocator_traits<allocator_type>::deallocate(alloc_, p, n);
	}

	template <typename ...Args>
	void construct(pointer p, Args &&...args)
	{
		std::allocator_traits<allocator_type>::construct(alloc_, p, 
			std::forward<Args>(args)...);
	}

	void destroy(pointer p)
	{
		std::allocator_traits<allocator_type>::destroy(alloc_, p);
	}

	void range_destroy(pointer first, pointer last)
	{
		while (last != first)
			destroy(--last);
	}

	void initialize()
	{
		start_ = finish_ = end_of_storage_ = NULL;
	}

	void initialize(size_type n)
	{
		start_ = finish_ = allocate(n);
		end_of_storage_ = start_+n;
	}

	void finalize()
	{
		range_destroy(start_, finish_);
		deallocate(start_, capacity());
	}
};

/**
 * Relational operators for vector
 * Performs the appropriate comparison operation between the vector containers 
 * lhs and rhs.
 */
template <typename T, typename Alloc>
bool operator ==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline
bool operator !=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator <(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
bool operator >(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return (rhs < lhs);
}

template <typename T, typename Alloc>
bool operator <=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(lhs > rhs);
}

template <typename T, typename Alloc>
bool operator >=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(lhs < rhs);
}

/**
 * Exchange contents of vectors
 * The contents of container x are exchanged with those of y. 
 * Both container objects must be of the same type (same template parameters), 
 * although sizes may differ.
 */
template <typename T, typename Alloc>
inline
void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
{
	return x.swap(y);
}

}	// namespace mini_stl

#endif // MINI_STL_VECTOR_INC

