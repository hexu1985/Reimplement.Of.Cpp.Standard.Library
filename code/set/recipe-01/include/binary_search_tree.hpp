// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef HX_BINARY_SEARCH_TREE_INC
#define HX_BINARY_SEARCH_TREE_INC

#ifdef __cplusplus
#include <cstddef>
#include <cassert>
#else // ! __cplusplus
#include <stddef.h>
#include <assert.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace Hx {
namespace binary_search {
#endif // __cplusplus

// 二叉搜索树节点, 包含指向父节点的指针,
// 以及指向左右子节点的指针,
// 不包含数据
typedef struct tree_node_t {
    tree_node_t* parent;
    tree_node_t* left;
    tree_node_t* right;
} tree_node_t;

// 初始化二叉搜素树节点
inline
void tree_node_init(tree_node_t* x)
{
    x->parent = x->left = x->right = NULL; 
}

// 设置左子节点, p -> parent, l -> left child
inline
void tree_set_left_child(tree_node_t* p, tree_node_t* l)
{
    p->left = l;
    if (l != NULL) l->parent = p;
}

// 设置右子节点, p -> parent, r -> right child
inline
void tree_set_right_child(tree_node_t* p, tree_node_t* r)
{
    p->right = r;
    if (r != NULL) r->parent = p;
}

// 返回以节点x为根的子树的最小元素的指针,
inline
tree_node_t* tree_minimum(tree_node_t* x)
{
    while (x->left != NULL)
        x = x->left;
    return x;
}

// 返回以节点x为根的子树的最大元素的指针,
inline
tree_node_t* tree_maximum(tree_node_t* x)
{
    while (x->right != NULL)
        x = x->right;
    return x;
}

// 返回以节点x的后继节点
inline
tree_node_t* tree_successor(tree_node_t* x)
{
    if (x->right != NULL)
        return tree_minimum(x->right);

    tree_node_t* y = x->parent;
    while (y != NULL && x == y->right) {
        x = y;
        y = y->parent;
    }    
    return y;
}

// 返回以节点x的前驱节点
inline
tree_node_t* tree_predecessor(tree_node_t* x)
{
    if (x->left != NULL)
        return tree_maximum(x->left);

    tree_node_t* y = x->parent;
    while (y != NULL && x == y->left) {
        x = y;
        y = y->parent;
    }    
    return y;
}

// 返回以节点x为根节点的子树的节点个数
inline
int tree_size(tree_node_t* x)
{
    if (x == NULL)
        return 0;

    return 1+tree_size(x->left)+tree_size(x->right);
}

// 二叉搜索树
typedef struct tree_t {
    tree_node_t* root;
} tree_t;

// 初始化二叉搜索树
inline
void tree_init(tree_t* tree)
{
    tree->root = NULL;
}

// 设置二叉树的根节点
inline 
void tree_set_root(tree_t* tree, tree_node_t* root)
{
    assert(root != NULL);
    tree->root = root;
    root->parent = NULL;
}

// 判断二叉搜索树是否为空,
inline
bool tree_is_empty(const tree_t* tree)
{
    return (tree->root == NULL);
}

/**
 * 用另一棵子树替换一棵子树并成为其父节点的孩子节点:
 * 用一个以v为根的子树来替换一棵以u为根的子树时,
 * 节点u的父节点就变为v的父节点, 并且最后v成为u的父节点的相应孩子.
 *
 *         q           
 *         |         |
 *         B u       E v
 *        / \       / \
 *       A   C     D   F
 *
 *              ||
 *              \/
 *         q           
 *         |         |  
 *         E v       B u
 *        / \       / \
 *       D   F     A   C
 *
 */
inline
void tree_transplant(tree_t* tree, tree_node_t* u, tree_node_t* v)
{
    if (u->parent == NULL) {            // u为树的根节点
        tree->root = v;
    } else if (u == u->parent->left) {  // u为父节点的左子树
        u->parent->left = v;
    } else {                            // u为父节点的右子树
        u->parent->right = v;
    }

    if (v != NULL)
        v->parent = u->parent;
}

/**
 * 从一棵二叉搜索树中删除一个节点z, 整个策略分为三种基本情况:
 * 1)如果z没有孩子节点, 那么只是简单的将它删除, 并修改它的父节点, 
 *   用NIL作为孩子来替换z;
 * 2)如果z只有一个孩子, 那么将这个孩子提升到树中z的位置上, 
 *   并修改z的父节点, 用z的孩子还替换z;
 * 3)如果z有两个孩子, 那么找z的后继y(一定在z的右子树中),
 *   并让y占据树中z的位置; z的原来右子树部分成为y的新的右子树,
 *   并且z的左子树成为y的新的左子树.
 *
 * a): 情况1和情况2的一部分(z只有一个右孩子)
 *        q             q
 *        |             |
 *        A z  =====>   B r
 *       / \           / \
 *     NIL  B r
 *         / \
 *
 * b): 情况2的另一部分(z只有一个左孩子)
 *        q             q
 *        |             |
 *        B z  =====>   A l
 *       / \           / \
 *    l A  NIL
 *     / \
 *
 * c): 情况3的一部分(z的后继y正好为z的右孩子)
 *           q                       q
 *           |                       |
 *           B z                     C y
 *        __/ \__     =====>      __/ \__
 *       /       \               /       \
 *    l A         C y         l A         D x
 *    /   \     /   \         /   \     /   \
 *            NIL    D x
 *                  / \
 *
 * d): 情况3的另一部分(z的后继y不是z的右孩子)
 *         q                       q                             q
 *         |                       |                             |
 *         B z                     B z       C y                 C y
 *     __/   \__    ======>    __/         /   \   =====>    __/   \__
 *    /         \             /          NIL    E r         /         \
 * l A           E r       l A                /   \      l A           E r
 * /   \       /   \       /   \           x D           /   \       /   \
 *          y C                             / \                   x D
 *          /   \                                                  / \
 *        NIL  x D                   
 *              / \
 *
 */
inline
void tree_delete(tree_t* tree, tree_node_t* z)
{
    if (z->left == NULL)
        tree_transplant(tree, z, z->right);     // a)
    else if (z->right == NULL)
        tree_transplant(tree, z, z->left);      // b)
    else {
        tree_node_t* y = tree_minimum(z->right);
        if (y->parent != z) {
            tree_transplant(tree, y, y->right); // d)
            y->right = z->right;                // d)
            y->right->parent = y;               // d)
        }
        tree_transplant(tree, z, y);            // c)
        y->left = z->left;                      // c)
        y->left->parent = y;                    // c)
    }
}

#ifdef __cplusplus
} // namespace binary_search
} // namespace Hx
#endif // __cplusplus

#endif // HX_BINARY_SEARCH_TREE_INC
