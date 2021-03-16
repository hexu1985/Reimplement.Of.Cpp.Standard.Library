// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_LIST_IMPL_INC
#define MINI_STL_LIST_IMPL_INC

#ifdef __cplusplus
#include <cstddef>
#else // ndef __cplusplus
#include <stddef.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace mini_stl {
namespace detail {
#endif // __cplusplus

/**
 * Doubly linked node
 */
typedef struct list_node_t {
	struct list_node_t *next;
	struct list_node_t *prev;
} list_node_t;

/**
 * Doubly linked node
 */
typedef struct list_t {
	list_node_t nil;
} list_t;

/**
 * initialize an circular doubly linked list
 *   ____
 *  |    |
 *  |    v
 *  `--[nil]--.
 *       ^    |
 *       |____|
 */
inline
void list_init(list_t *lst)
{
	lst->nil.next = &lst->nil;
	lst->nil.prev = &lst->nil;
}

/**
 * get list first node link
 */
inline
list_node_t *list_head(const list_t *lst)
{
	return (list_node_t *) lst->nil.next;
}

/**
 * get list last node link
 */
inline
list_node_t *list_tail(const list_t *lst)
{
	return (list_node_t *) lst->nil.prev;
}

/**
 * get after last node link
 */
inline
list_node_t *list_after_tail(const list_t *lst)
{
	return (list_node_t *) &lst->nil;
}

/**
 * insert a node before pos
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [nil]
 *                     ^-pos
 *                  ||
 *                  \/
 * [nil] <=> [N1] <=> [M] <=> [N2] <=> [N3] <=> [nil]
 *                     ^-node   ^-pos
 */
inline
void list_insert(list_node_t *pos, list_node_t *node)
{
	node->prev = pos->prev;
	node->next = pos;
	pos->prev->next = node;
	pos->prev = node;
}

/**
 * insert [first, last) before pos
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [nil]
 *                      ^-pos
 * [nil] <=> [M1] <=> [...] <=> [Mm] <=> [Mn] <=> [nil]
 *             ^-first            ^-keep   ^-last
 *                    ||
 *                    \/
 * [nil] <=> [N1] <=> [M1] <=> [...] <=> [Mm] <=> [N2] <=> [N3] <=> [nil] 
 *                      ^-first            ^-keep   ^-pos
 */
inline
void list_range_insert(list_node_t *pos, list_node_t *first, list_node_t *last)
{
	if (first == last)
		return;

	first->prev = pos->prev;
	last->prev->next = pos;
	pos->prev->next = first;
	pos->prev = last;
}

/**
 * remove node from list
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [nil]
 *                      ^-pos
 *                  ||
 *                  \/
 * [nil] <=> [N1] <=> [N3] <=> [nil]
 */
inline
void list_remove(list_node_t *pos)
{
	pos->prev->next = pos->next;
	pos->next->prev = pos->prev;
}

/**
 * remove [first, last) from list
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [N4] <=> [N5] <=> [nil]
 *                      ^-first           ^-last
 *                  ||
 *                  \/
 * [nil] <=> [N1] <=> [N4] <=> [N5] <=> [nil]
 */
inline 
void list_range_remove(list_node_t *first, list_node_t *last)
{
	if (first == last) 
		return;

	first->prev->next = last;
	last->prev = first->prev;
}

/**
 * transfer [first, last) before pos
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [nil]
 *                      ^-pos
 * [nil] <=> [M1] <=> [...] <=> [Mm] <=> [Mn] <=> [nil]
 *             ^-first            ^-keep   ^-last
 *                    ||
 *                    \/
 * [nil] <=> [N1] <=> [M1] <=> [...] <=> [Mm] <=> [N2] <=> [N3] <=> [nil] 
 *                      ^-first            ^-keep   ^-pos
 */
inline
void list_transfer(list_node_t *pos, list_node_t *first, list_node_t *last)
{
	if (first == last) 
		return;

	list_node_t *keep = last->prev;	// keep the node before last

	first->prev->next = last;
	last->prev = first->prev;

	first->prev = pos->prev;
	keep->next = pos;
	pos->prev->next = first;
	pos->prev = keep;
}

/**
 * test if list is empty
 */
inline 
int list_is_empty(const list_t *lst)
{
	return (lst->nil.next == &lst->nil);
}

/**
 * reverse circular doubly linked list 
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> [N4] <=> [N5] <=> [nil]
 *                      ^-first                    ^-last
 *                      ^-head   ^-keep
 *                ||
 *                \/
 * [nil] <=> [N1] <=> [N3] <=> [N2] <=> [N4] <=> [N5] <=> [nil]
 *                               ^-first  ^-keep   ^-last
 *                      ^head
 *
 */
inline
void list_reverse(list_node_t *first, list_node_t *last)
{
	if (first == last) 
		return;

	list_node_t *head = first;
	list_node_t *keep = first->next;;
	for ( ; keep != last; keep = first->next) {
		first->next = keep->next;	// remove keep from list
		keep->next->prev = first;

		keep->next = head;		// insert keep before head
		keep->prev = head->prev;
		head->prev->next = keep;
		head->prev = keep;

        head = head->prev;
	}
}

/**
 * get size of [first, last)
 */
inline
size_t list_size(const list_t *lst)
{
	const list_node_t *first = lst->nil.next;
	const list_node_t *last = &lst->nil;
	size_t n = 0;
	while (first != last) {
		first = first->next;
		n++;
	}
	return n;
}

/**
 * swap two list
 */
inline
void list_swap(list_t *a, list_t *b)
{
	list_t tmp;
	list_init(&tmp);
	list_transfer(list_after_tail(&tmp), list_head(a), list_after_tail(a));
	list_transfer(list_after_tail(a), list_head(b), list_after_tail(b));
	list_transfer(list_after_tail(b), list_head(&tmp), list_after_tail(&tmp));
}

#ifdef __cplusplus
/**
 * search first equal key's node in [first, last)
 */
template <typename T, typename BinaryPredicate, typename UnaryFunction>
list_node_t *list_search(const list_node_t *first, const list_node_t *last, 
	const T &key, BinaryPredicate equal, UnaryFunction get_key)
{
	while (first != last && !equal(get_key(first), key)) {
		first = first->next;
	}
	return (list_node_t *) first;
}

/**
 * search first node in [first, last) which pred return true
 */
template <typename UnaryPredicate, typename UnaryFunction>
list_node_t *list_search(const list_node_t *first, const list_node_t *last, 
	UnaryPredicate pred, UnaryFunction get_key)
{
	while (first != last && !pred(get_key(first))) {
		first = first->next;
	}
	return (list_node_t *) first;
}

/**
 * call op on range [fist, last)
 */
template <typename UnaryOperation>
void list_foreach(list_node_t *first, list_node_t *last, UnaryOperation op)
{
	list_node_t *keep;
	while (first != last) {
		keep = first->next;
		op(first);
		first = keep;
	}
}

template <typename UnaryOperation, typename UnaryFunction>
void list_foreach(list_node_t *first, list_node_t *last, 
	UnaryOperation op, UnaryFunction get_key)
{
	while (first != last) {
		op(get_key(first));
		first = first->next;
	}
}

/**
 * destroy list
 */
template <typename UnaryOperation>
void list_destroy(list_t *lst, UnaryOperation destroy)
{
	list_foreach(lst->nil.next, &lst->nil, destroy);
}

/**
 * merge src list [src_pos, src_nil) to dst list [dst_pos, dst_nil), 
 * assume both list range are sorted
 */
template <typename BinaryPredicate, typename UnaryFunction>
void list_merge(list_node_t *dst_pos, list_node_t *dst_nil, 
	list_node_t *src_pos, list_node_t *src_nil, 
	BinaryPredicate less, UnaryFunction get_key)
{
	while (dst_pos != dst_nil && src_pos != src_nil) {
		// find first greater than src_pos in dst list, 
		// is the insert pos in dst list
		while (dst_pos != dst_nil && !less(get_key(src_pos), get_key(dst_pos)))
			dst_pos = dst_pos->next;
		if (dst_pos == dst_nil) // nofound
			break;

		// find first pos greater than dst_pos in src list,
		// range [src_beg, src_pos) is the transfer range
		list_node_t *src_beg = src_pos;
		while (src_pos != src_nil && !less(get_key(dst_pos), get_key(src_pos)))
			src_pos = src_pos->next;

		// transfer [src_beg, src_pos) insert front dst_pos
		list_transfer(dst_pos, src_beg, src_pos);

		// because dst_pos's key < src_pos's key, move dst_pos to next
		dst_pos = dst_pos->next;
	}
	
	// transfer src list's remainders to dst list tail if need
	if (dst_pos == dst_nil) {
		list_transfer(dst_nil, src_pos, src_nil);
	}
}

/**
 * remove duplicate values in list
 */
template <typename BinaryPredicate, typename UnaryFunction, 
	typename UnaryOperation>
void list_unique(list_node_t *first, list_node_t *last, 
	BinaryPredicate equal, UnaryFunction get_key, UnaryOperation destroy)
{
	while (first != last) {
		// find pos that not equal first's value
		list_node_t *pos = first->next;
		while (pos != last && equal(get_key(pos), get_key(first)))
			pos = pos->next;
		list_node_t *beg = first->next;
		list_range_remove(beg, pos);
		list_foreach(beg, pos, destroy);
		first = pos;
	}
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace detail
} // namespace mini_stl
#endif // __cplusplus

#endif // MINI_STL_LIST_IMPL_INC

