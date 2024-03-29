// -*- C++ -*-
// HeXu's
// 2013 Dec

#ifndef HX_SINGLY_LINKED_LIST_INC
#define HX_SINGLY_LINKED_LIST_INC

#ifdef __cplusplus
#include <cstddef>
#include <cassert>
#else
#include <stddef.h>
#include <assert.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace Hx {
namespace singly_linked {
#endif // __cplusplus

/**
 * 单向链表节点
 *    ___     
 *   |   |------>
 *   |___| next  
 * 
 */
struct list_node_t {
    list_node_t* next = nullptr;
};

/**
 * 单链表: 有哑元头节点, 尾节点的next为空
 *
 * [dummy] -> [N1] -> [N2] -> ... -> NIL
 *
 */
struct list_t {
    list_node_t dummy;
};

/**
 * 初始化链表
 *
 * [dummy] -> NIL
 *
 */
inline
void list_init(list_t* list)
{
    list->dummy.next = nullptr;
}

// 返回链表头部第一个节点(非哑元头节点)的指针
inline
list_node_t* list_head(const list_t* list)
{
    return (list_node_t* ) list->dummy.next;
}

// 返回链表哑元头节点的指针
inline
list_node_t* list_before_head(const list_t* list)
{
    return (list_node_t* ) &(list->dummy);
}

// 判断链表是否为空
inline
int list_is_empty(const list_t* list)
{
    return (list->dummy.next == NULL);
}

/**
 * 在链表的x节点后插入t节点
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
void list_insert_after(list_node_t* x, list_node_t* t)
{
    t->next = x->next;
    x->next = t;
}

/**
 * 从链表上删除x节点后面的节点
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
list_node_t* list_delete_after(list_node_t* x)
{
    list_node_t* t = x->next;
    if (t != NULL)
        x->next = t->next;    
    return t;
}

// 删除链表头节点
inline
list_node_t* list_delete_head(list_t* list)
{
    return list_delete_after(list_before_head(list));
}

// 返回链表中元素个数
inline
size_t list_size(const list_t* list)
{
    size_t n = 0;
    list_node_t* node = list_head(list);
    while (node != NULL) {
        node = node->next;
        n++;
    }
    return n;
}

/**
 * 将链表上x节点后面的所有节点反序排列
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
void list_reverse_after(list_node_t* x)
{
    if (x == NULL || x->next == NULL)
        return;

    list_node_t* y = x->next;
    while (y->next != NULL) {
        list_node_t* t = list_delete_after(y);
        list_insert_after(x, t);
    }
}

#ifdef __cplusplus
} // namespace singly_linked
} // namespace Hx
#endif // __cplusplus

#endif // HX_SINGLY_LINKED_LIST_INC


