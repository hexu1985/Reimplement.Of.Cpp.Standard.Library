// -*- C++ -*-
// HeXu's
// 2013 Dec

#ifndef MINI_STL_FORWARD_LIST_IMPL_H
#define MINI_STL_FORWARD_LIST_IMPL_H

#ifdef __cplusplus
#include <cstddef>
#include <cassert>
#else
#include <stddef.h>
#include <assert.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace mini_stl {
namespace single_linked {
#endif // __cplusplus

/**
 * 单向链表节点
 *    ___     
 *   |   |------>
 *   |___| next  
 * 
 */
typedef struct list_node_t {
	struct list_node_t *next;
} list_node_t;

/**
 * 单链表: 有哑元头结点, 尾结点的next为空
 *
 * [head] -> [N1] -> [N2] -> ... -> NIL
 *
 */
typedef struct list_t {
	list_node_t head;
} list_t;

/**
 * 初始化链表
 *
 * [head] -> NIL
 *
 */
inline
void list_init(list_t *lst)
{
	lst->head.next = NULL;
}

// 返回链表头部第一个结点(非哑元头结点)的指针
inline
list_node_t *list_head(const list_t *lst)
{
	return (list_node_t *) lst->head.next;
}

// 返回链表哑元头结点的指针
inline
list_node_t *list_before_head(const list_t *lst)
{
	return (list_node_t *) &lst->head;
}

// 判断链表是否为空
inline
int list_is_empty(const list_t *lst)
{
	return (lst->head.next == NULL);
}

/**
 * 在链表的x结点后插入t结点
 *            ___                ___
 *    ------>|   |------------->|   |------>
 *           |___|              |___|
 *             ^-x
 *                     ___        
 *                    |   |------>
 *                    |___|       
 *                      ^-t       
 * =========================================
 *            ___                ___
 *    ------>|   |------------->|   |------>
 *           |___|          .-->|___|
 *             ^-x          |
 *                     ___  |(1) 
 *                    |   |-'
 *                    |___|       
 *                      ^-t       
 * =========================================
 *            ___                ___
 *    ------>|   |--.        .->|   |------>
 *           |___|  |(2)     |  |___|
 *             ^-x  |        |
 *                  |   ___  |      
 *                  '->|   |-'
 *                     |___|       
 *                       ^-t       
 */
inline
void list_insert_after(list_node_t *x, list_node_t *t)
{
	t->next = x->next;
	x->next = t;
}

/**
 * 从链表上删除x结点后面的结点
 *            ___           ___           ___ 
 *    ------>|   |-------->|   |-------->|   |------>
 *           |___|         |___|         |___|
 *             ^-x           ^-t
 * ==================================================
 *                     ____________________
 *                    |                    |
 *            ___     |(1)  ___           _V_ 
 *    ------>|   |----'    |   |-------->|   |------>
 *           |___|         |___|         |___|
 *             ^-x           ^-t
 */
inline
list_node_t *list_delete_after(list_node_t *x)
{
	list_node_t *t = x->next;
	if (t != NULL)
		x->next = t->next;	
	return t;
}

// 返回链表中元素个数
inline
size_t list_size(const list_t *lst)
{
	size_t n = 0;
	list_node_t *node = list_head(lst);
	while (node != NULL) {
		node = node->next;
		n++;
	}
	return n;
}

// 交换两个链表
inline
void list_swap(list_t *a, list_t *b)
{
    list_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * 将链表上x结点后面的所有结点反序排列
 *
 * [head] -> [N1] -> [N2] -> [N3] -> [N4] -> [NIL]
 *    ^-x      ^-y    ^-t
 *                || 
 *                \/
 *
 * [head] -> [N2] -> [N1] -> [N3] -> [N4] -> [NIL]
 *    ^-x             ^-y     ^-t
 *                ||  
 *                \/
 *
 * [head] -> [N3] -> [N2] -> [N1] -> [N4] -> [NIL]
 *    ^-x                     ^-y     ^-t
 *                ||  
 *                \/
 *
 * [head] -> [N4] -> [N3] -> [N2] -> [N1] -> [NIL]
 *    ^-x                              ^-y
 */
inline
void list_reverse_after(list_node_t *x)
{
    if (x == NULL || x->next == NULL)
        return;

	list_node_t *y = x->next;
    while (y->next != NULL) {
        list_node_t *t = list_delete_after(y);
        list_insert_after(x);
    }
}

#ifdef __cplusplus
} // namespace single_linked
} // namespace mini_stl
#endif // __cplusplus

#endif // MINI_STL_FORWARD_LIST_IMPL_H


