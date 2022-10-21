// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef HX_DOUBLY_LINKED_LIST_INC
#define HX_DOUBLY_LINKED_LIST_INC

#ifdef __cplusplus
#include <cstddef>
#else // ndef __cplusplus
#include <stddef.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace Hx {
namespace doubly_linked {
#endif // __cplusplus

/**
 * 双向循环链表节点, 不包含数据
 *            ___  next      
 *           |   |------>
 *     <-----|___|       
 *       prev
 */
typedef struct list_node_t {
    struct list_node_t* next;
    struct list_node_t* prev;
} list_node_t;

/**
 * 双向循环链表
 */
typedef struct list_t {
    list_node_t nil;
} list_t;

/**
 * 初始化NIL节点
 *         ____
 *        |    |
 *        V    |
 *   .--[nil]--'
 *   |    A    
 *   |____|     
 */
inline
void list_nil_init(list_node_t* nil)
{
    nil->prev = nil->next = nil;
}

/**
 * 初始化链表
 */
inline
void list_init(list_t* list)
{
    list_nil_init(&list->nil);
}

/**
 * 获取链表头节点指针
 */
inline
list_node_t* list_head(const list_t* list)
{
    return (list_node_t*) list->nil.next;
}

/**
 * 获取链表尾节点指针
 */
inline
list_node_t* list_tail(const list_t* list)
{
    return (list_node_t*) list->nil.prev;
}

/**
 * 获取NIL节点指针
 */
inline
list_node_t* list_nil(const list_t* list)
{
    return (list_node_t*) &list->nil;
}

/**
 * 在链表的x节点前插入t节点
 *            ___                ___
 *           |   |------------->|   |------>
 *    <------|___|<-------------|___|
 *                                ^-x
 *                  ___        
 *                 |   |------>
 *            <----|___|       
 *                   ^-t       
 * =========================================
 *            ___                ___
 *           |   |------------->|   |------>
 *    <------|___|<-------------|___|
 *             A                 A ^-x
 *             |(1) ___          |(2)
 *             |   |   |---------'
 *             '---|___|       
 *                   ^-t       
 * =========================================
 *            ___                ___
 *           |   |--.     .---->|   |------>
 *    <------|___|  |(3)  |  .--|___|
 *             A    |     |  |    ^-x
 *             |    V__   |  |(4)
 *             |   |   |--'  |
 *             '---|___|<----'
 *                   ^-t
 */
inline
void list_insert(list_node_t* x, list_node_t* t)
{
	t->prev = x->prev;      // (1)
	t->next = x;            // (2)
	t->prev->next = t;      // (3)
	t->next->prev = t;      // (4)
}

/**
 * 在链表的x节点前插入[a,b]节点
 *            ___                           ___
 *           |   |------------------------>|   |------>
 *    <------|___|<------------------------|___|
 *                                           ^-x
 *                  ___         ___        
 *                 |   |------>|   |------>
 *            <----|___|<------|___|       
 *                   ^-a         ^-b
 * =====================================================
 *            ___                           ___
 *           |   |------------------------>|   |------>
 *    <------|___|<------------------------|___|
 *             A                            A ^-x
 *             |(1) ___         ___         |(2)
 *             |   |   |------>|   |--------'
 *             '---|___|<------|___|      
 *                   ^-a         ^-b      
 * =====================================================
 *            ___                            ___
 *           |   |--.                 .---->|   |------>
 *    <------|___|  |(3)              |  .--|___|
 *             A    |              (4)|  |    ^-x
 *             |    V__         ___   |  |
 *             |   |   |------>|   |--'  |
 *             '---|___|<------|___|<----'
 *                   ^-a         ^-b
 */
inline
void list_insert_range(list_node_t* x, list_node_t* a, list_node_t* b)
{
	a->prev = x->prev;      // (1)
	b->next = x;            // (2)
	a->prev->next = a;      // (3)
	b->next->prev = b;      // (4)
}

/**
 * 从链表上删除x节点
 *            ___           ___           ___ 
 *           |   |-------->|   |-------->|   |------>
 *    <------|___|<--------|___|<--------|___|
 *                           ^-x       
 * ==================================================
 *                    .--------------------.
 *                    |(1)                 |
 *            ___     |     ___           _V_ 
 *           |   |----'    |   |-------->|   |------>
 *    <------|___|<--------|___|    .----|___|
 *             A             ^-x    |   
 *             |                    |(2)
 *             '--------------------'
 */
inline
void list_delete(list_node_t* x)
{
	x->prev->next = x->next;    // (1)
	x->next->prev = x->prev;    // (2)
}

/**
 * 从链表上删除[a,b]节点
 *            ___           ___           ___           ___ 
 *           |   |-------->|   |-------->|   |-------->|   |------>
 *    <------|___|<--------|___|<--------|___|<--------|___|
 *                           ^-a           ^-b         
 * =================================================================
 *                    .----------------------------------.
 *                    |(1)                               |
 *            ___     |     ___           ___           _V_ 
 *           |   |----'    |   |-------->|   |-------->|   |------>
 *    <------|___|<--------|___|<--------|___|    .----|___|
 *             A             ^-a           ^-b    |   
 *             |                                  |(2)
 *             '----------------------------------'
 */
inline
void list_delete_range(list_node_t* a, list_node_t* b)
{
	a->prev->next = b->next;    // (1)
	b->next->prev = a->prev;    // (2)
}

/**
 * 将t节点移动到x节点前(x, t可以在同一个链表或不同链表上)
 * [nil] <=> [N1] <=> [N2] <=> [N3] <=> ... <=> [Nk-1] <=> [Nk] <=> [Nk+1] <=> ...
 *                              ^-x                         ^-t
 *                               ||
 *                               \/
 *
 * [nil] <=> [N1] <=> [N2] <=> [Nk] <=> [N3] <=> ... <=> [Nk-1] <=> [Nk+1] <=> ... 
 *                              ^-t      ^-x
 */
inline
void list_transfer(list_node_t* x, list_node_t* t)
{
    list_delete(t);            // 移除节点t
    list_insert(x, t);         // 将t节点插入到x节点前
}

/**
 * 将a节点和b节点之间的节点(包括a和b节点)插入到x节点后, 调用者必须保证[a, b]为有效区间
 * [head] <=> [N1] <=> [N2] <=> [N3] <=> ... <=> [NIL]
 *                               ^-x
 *
 * [head] <=> [M1] <=> [M2] <=> [M3] <=> ... <=> [Mk-1] <=> [Mk] <=> [Mk+1] <=> ... <=> [NIL]
 *                               ^-a              ^-b 
 *                                     ||
 *                                     \/
 *
 * [head] <=> [N1] <=> [N2] <=> [M3] <=> ... <=> [Mk-1] <=> [N3] <=> ... <=> [NIL]
 *                               ^-a              ^-b        ^-x
 *
 * [head] <=> [M1] <=> [M2] <=> [Mk] <=> [Mk+1] <=> ... <=> [NIL]
 *
 */
inline
void list_transfer_range(list_node_t* x, list_node_t* a, list_node_t* b)
{
    list_delete_range(a, b);
    list_insert_range(x, a, b);
}

/**
 * 检查链表是否为空
 */
inline 
int list_is_empty(const list_t* list)
{
    return (list->nil.next == &list->nil);
}

/**
 * 将链表上[x, nil)之间的所有节点反序排列
 *
 * [head] <=> [N1] <=> [N2] <=> [N3] <=> [N4] <=> [NIL]
 *    ^-y       ^-x       
 *                ||  list_transfer(y->next, x->next)
 *                \/
 *
 * [head] <=> [N2] <=> [N1] <=> [N3] <=> [N4] <=> [NIL]
 *    ^-y                ^-x
 *                ||  list_transfer(y->next, x->next)
 *                \/
 *
 * [head] <=> [N3] <=> [N2] <=> [N1] <=> [N4] <=> [NIL]
 *    ^-y                         ^-x
 *                ||  list_transfer(y->next, x->next)
 *                \/
 *
 * [head] <=> [N4] <=> [N3] <=> [N2] <=> [N1] <=> [NIL]
 *    ^-y                                  ^-x
 */
inline
void list_reverse(list_node_t* x, list_node_t* nil)
{
    if (x == nil) return;

    list_node_t* y = x->prev;
    while (x->next != nil) {
        list_transfer(y->next, x->next);
    }
}

/**
 * 获取列表大小
 */
inline
size_t list_size(const list_t* list)
{
    const list_node_t* x = list->nil.next;
    const list_node_t* nil = &list->nil;
    size_t n = 0;
    while (x != nil) {
        x = x->next;
        n++;
    }
    return n;
}

#ifdef __cplusplus
} // namespace doubly_linked
} // namespace Hx
#endif // __cplusplus

#endif // HX_DOUBLY_LINKED_LIST_INC

