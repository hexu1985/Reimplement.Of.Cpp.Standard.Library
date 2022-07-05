// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_SET_IMPL_INC
#define MINI_STL_SET_IMPL_INC

#ifdef __cplusplus
#include <cstddef>
#include <cassert>
#else // ! __cplusplus
#include <stddef.h>
#include <assert.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace Hx {
namespace red_black {
#endif // __cplusplus

// 红黑树节点颜色
typedef enum tree_node_color_t {
    kRed = 0,
    kBlack = 1,
} tree_node_color_t;

// 红黑树节点, 包含指向父节点的指针,
// 以及指向左右子节点的指针,
// 不包含数据
typedef struct tree_node_t {
    tree_node_t* parent;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_color_t color;
} tree_node_t;

// 红黑树, 满足如下性质的二叉搜索树:
// 1) 每个节点或是红色的, 或是黑色的;
// 2) 根节点是黑色的;
// 3) 每个叶节点(NIL)是黑色的;
// 4) 如果一个节点是红色的, 则它的两个子节点都是黑色的,
//    (任何路径上不存在连续的黑色节点);
// 5) 对每个节点, 从该节点到其所有后代叶节点的简单路径上,
//    均包含相同数目的黑色节点.
typedef struct tree_t {
    tree_node_t* root;
    tree_node_t nil;
} tree_t;

// 初始化红黑树节点
inline
void tree_node_init(tree_node_t* x)
{
    x->parent = x->left = x->right = NULL; 
    x->color = kRed;
}

// 设置左子节点, p -> parent, l -> left child
inline
void tree_set_left_child(tree_node_t* p, tree_node_t* l)
{
    p->left = l;
    l->parent = p;
}

// 设置右子节点, p -> parent, r -> right child
inline
void tree_set_right_child(tree_node_t* p, tree_node_t* r)
{
    p->right = r;
    r->parent = p;
}

// 初始化红黑树NIL节点
inline
void tree_nil_init(tree_node_t *nil)
{
    nil->parent = nil->left = nil->right = nil;
    nil->color = kBlack;
}

// 初始化红黑树
inline
void tree_init(tree_t* tree)
{
    tree_nil_init(&tree->nil);
    tree->root = &tree->nil;
}

// 设置红黑树的根节点
inline 
void tree_set_root(tree_t* tree, tree_node_t* root)
{
    assert(root != NULL);
    tree->root = root;
    root->parent = &tree->nil;
}

// 返回以节点x为根的子树的最小元素的指针,
inline
tree_node_t* tree_minimum(tree_t *tree, tree_node_t* x)
{
    tree_node_t* nil = &tree->nil;
    while (x->left != nil)
        x = x->left;
    return x;
}

// 返回以节点x为根的子树的最大元素的指针,
inline
tree_node_t* tree_maximum(tree_t *tree, tree_node_t* x)
{
    tree_node_t* nil = &tree->nil;
    while (x->right != nil)
        x = x->right;
    return x;
}

// 返回以节点x的后继节点
inline
tree_node_t* tree_successor(tree_t *tree, tree_node_t* x)
{
    tree_node_t* nil = &tree->nil;
    if (x->right != nil)
        return tree_minimum(tree, x->right);

    tree_node_t* y = x->parent;
    while (y != nil && x == y->right) {
        x = y;
        y = y->parent;
    }    
    return y;
}

// 返回以节点x的前驱节点
inline
tree_node_t* tree_predecessor(tree_t *tree, tree_node_t* x)
{
    tree_node_t* nil = &tree->nil;
    if (x->left != nil)
        return tree_maximum(tree, x->left);

    tree_node_t* y = x->parent;
    while (y != nil && x == y->left) {
        x = y;
        y = y->parent;
    }    
    return y;
}

// 返回以节点x为根节点的子树的节点个数
inline
int tree_size(const tree_t *tree, const tree_node_t* x)
{
    if (x == &tree->nil)
        return 0;

    return 1+tree_size(tree, x->left)+tree_size(tree, x->right);
}

// 判断红黑树是否为空,
inline
bool tree_is_empty(const tree_t* tree)
{
    return (tree->root == &tree->nil);
}

/**
 * 在节点x上做左旋
 *
 *        |                     |
 *        x                     y
 *       / \       =====>      / \
 *      a   y                 x   c
 *         / \               / \
 *        b   c             a   b
 */
inline
void tree_left_rotate(tree_t *tree, tree_node_t* x)
{
    tree_node_t* y = x->right;
    x->right = y->left;
    if (y->left != &tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == &tree->nil) {      // x是根节点
        tree->root = y;
    } else if (x == x->parent->left) {  // x是左子树
        x->parent->left = y;
    } else {                            // x是右子树
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
    
/**
 * 在节点x上做右旋
 *
 *          |                 |
 *          x                 y 
 *         / \               / \
 *        y   c    =====>   a   x
 *       / \                   / \
 *      a   b                 b   c
 */
inline
void tree_right_rotate(tree_t *tree, tree_node_t* x)
{
    tree_node_t* y = x->left;
    x->left = y->right;
    if (y->right != &tree->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == &tree->nil) {      // x是根节点
        tree->root = y;
    } else if (x == x->parent->left) {  // x是左子树
        x->parent->left = y;
    } else {                            // x是右子树
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

/**
 * 插入节点后恢复搜索树的红黑性质
 * ()  -> 红色节点
 * []  -> 黑色节点
 * 
 * 1) z的叔节点y是红色的
 *            |                              |          
 *           [C]                      new z (C) 
 *        __/   \__                      __/   \__      
 *       /         \       =======>     /         \
 *     (A)         (D) y              [A]         [D]
 *    /   \       /   \              /   \       /   \
 *   a    (B) z  d     e            a    (B)    d     e 
 *       /   \                          /   \
 *      b     c                        b     c          
 *
 * 2) z的叔节点y是黑色的且z是一个右孩子
 *            |                                |
 *           [C]                              [C]
 *        __/   \__                        __/   \__
 *       /         \       =======>       /         \
 *     (A)          d y                 (B)          d y
 *    /   \                            /   \
 *   a    (B) z                    z (A)    c
 *       /   \                      /   \
 *      b     c                    a     b
 *
 * 3) z的叔节点y是黑色的且z是一个左孩子
 *               |                             |
 *              [C]                           [B]
 *           __/   \__                     __/   \__
 *          /         \     =======>      /         \
 *        (B)          d y            z (A)         (C)
 *       /   \                         /   \       /   \
 *   z (A)    c                       a     b     c     d
 *    /   \
 *   a     b
 */
inline
void tree_insert_fixup(tree_t *tree, tree_node_t* z) 
{
    tree_node_t* y = NULL;
    while (z->parent->color == kRed) {
        if (z->parent == z->parent->parent->left) {     // z的父节点为左子数
            y = z->parent->parent->right;
            if (y->color == kRed) {
                z->parent->color = kBlack;         // Case 1
                y->color = kBlack;                 // Case 1
                z->parent->parent->color = kRed;   // Case 1
                z = z->parent->parent;             // Case 1
            } else{
                if (z == z->parent->right) {
                    z = z->parent;                 // Case 2
                    tree_left_rotate(tree, z);     // Case 2
                }
                z->parent->color = kBlack;                  // Case 3
                z->parent->parent->color = kRed;            // Case 3
                tree_right_rotate(tree, z->parent->parent); // Case 3
            }
        } else {                                        // z的父节点为右子数
            y = z->parent->parent->left;
            if (y->color == kRed) {
                z->parent->color = kBlack;         // Case 1
                y->color = kBlack;                 // Case 1
                z->parent->parent->color = kRed;   // Case 1
                z = z->parent->parent;             // Case 1
            } else {
                if (z == z->parent->left) {
                    z = z->parent;                 // Case 2
                    tree_right_rotate(tree, z);    // Case 2
                }
                z->parent->color = kBlack;                  // Case 3
                z->parent->parent->color = kRed;            // Case 3
                tree_left_rotate(tree, z->parent->parent);  // Case 3
            }
        }
    }
    tree->root->color = kBlack;
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
    if (u->parent == &tree->nil) {      // u为树的根节点
        tree->root = v;
    } else if (u == u->parent->left) {  // u为父节点的左子树
        u->parent->left = v;
    } else {                            // u为父节点的右子树
        u->parent->right = v;
    }

    v->parent = u->parent;
}

/**
 * 删除结点后恢复搜索树的红黑性质
 *  ()  -> 红色结点
 *  []  -> 黑色结点
 * [[]] -> 双重黑色结点
 *
 * 1) x的兄弟结点w是红色的
 * 
 *                |                                 |
 *               [B]                               [D]
 *            __/   \__                         __/   \__
 *           /         \           =====>      /         \
 *      x [[A]]        (D) w                 (B)         [E]
 *        /   \       /   \                 /   \new w  /   \
 *       a     b    [C]   [E]         x [[A]]   [C]    e     f
 *                  / \   / \            / \    / \
 *                 c   d e   f          a   b  c   d   
 *
 * 2) x的兄弟结点w是黑色, 而且w的两个子结点都是黑色的
 *
 *                |                             |
 *                B c                     new x B c
 *            __/   \__                     __/   \__
 *           /         \        =====>     /         \
 *      x [[A]]        [D] w             [A]         (D)
 *        /   \       /   \             /   \       /   \
 *       a     b    [C]   [E]          a     b    [C]   [E]
 *                  / \   / \                     / \   / \
 *                 c   d e   f                   c   d e   f 
 *                                                        
 * 3) x的兄弟结点w是黑色的, w的左孩子是红色的, w的右孩子是黑色的
 *
 *                |                                |
 *                B c                              B c
 *            __/   \__                        __/   \__
 *           /         \        =====>        /         \
 *      x [[A]]        [D] w             x [[A]]        [C] new w
 *        /   \       /   \                /   \       /   \
 *       a     b    (C)   [E]             a     b     c    (D)
 *                  / \   / \                             /   \
 *                 c   d e   f                           d    [E]
 *                                                            / \
 *                                                           e   f
 *
 * 4) x的兄弟结点w是黑色的, 且w的右孩子是红色的
 *
 *                |                             |
 *                B c                           D c
 *            __/   \__                     __/   \__
 *           /         \        =====>     /         \
 *      x [[A]]        [D] w             [B]         [E]
 *        /   \       /   \             /   \        / \
 *       a     b     C c' (E)         [A]    C c'   e   f   
 *                  / \   / \         / \   / \
 *                 c   d e   f       a   b c   d    new x = T.root
 *
 */
inline
void tree_delete_fixup(tree_t* tree, tree_node_t* x)
{
    tree_node_t* w = NULL;
    while (x != tree->root && x->color == kBlack) {
        if (x == x->parent->left) {
            w = x->parent->right;
			if (w->color == kRed) {	                    
                w->color = kBlack;                 // Case 1
                x->parent->color = kRed;           // Case 1
				tree_left_rotate(tree, x->parent); // Case 1
                w = x->parent->right;              // Case 1
			}
            if (w->left->color == kBlack && w->right->color == kBlack) {
                w->color = kRed;                   // Case 2
                x = x->parent;                     // Case 2
            } else {
                if (w->right->color == kBlack) {
                    w->left->color = kBlack;       // Case 3
                    w->color = kRed;               // Case 3
                    tree_right_rotate(tree, w);    // Case 3
                    w = x->parent->right;          // Case 3
                }
                w->color = x->parent->color;       // Case 4
                x->parent->color = kBlack;         // Case 4
                w->right->color = kBlack;          // Case 4
                tree_left_rotate(tree, x->parent); // Case 4
                x = tree->root;                    // Case 4
            }
        } else {    // x是父节点的右子树
            w = x->parent->left;
			if (w->color == kRed) {	                    
                w->color = kBlack;                 // Case 1
                x->parent->color = kRed;           // Case 1
				tree_right_rotate(tree, x->parent);// Case 1
                w = x->parent->left;               // Case 1
			}
            if (w->left->color == kBlack && w->right->color == kBlack) {
                w->color = kRed;                   // Case 2
                x = x->parent;                     // Case 2
            } else {
                if (w->left->color == kBlack) {
                    w->right->color = kBlack;      // Case 3
                    w->color = kRed;               // Case 3
                    tree_left_rotate(tree, w);     // Case 3
                    w = x->parent->left;           // Case 3
                }
                w->color = x->parent->color;       // Case 4
                x->parent->color = kBlack;         // Case 4
                w->left->color = kBlack;           // Case 4
                tree_right_rotate(tree, x->parent);// Case 4
                x = tree->root;                    // Case 4
            }
        }
    }
    x->color = kBlack;
}

/**
 * 从一棵红黑树中删除一个结点z, 整个策略分为三种基本情况:
 * 1)如果z没有孩子结点, 那么只是简单的将它删除, 并修改它的父结点, 
 *   用NIL作为孩子来替换z;
 * 2)如果z只有一个孩子, 那么将这个孩子提升到树中z的位置上, 
 *   并修改z的父结点, 用z的孩子还替换z;
 * 3)如果z有两个孩子, 那么找z的后继y(一定在z的右子树中),
 *   并让y占据树中z的位置; z的原来右子树部分成为y的新的右子树,
 *   并且z的左子树成为y的新的左子树.
 *
 * a): 情况1和情况2的一部分(z只有一个右孩子)
 *        q             q
 *        |             |
 *      y A z  =====>   B x
 *       / \           / \
 *     NIL  B x
 *         / \
 *
 * b): 情况2的另一部分(z只有一个左孩子)
 *        q             q
 *        |             |
 *      y B z  =====>   A x
 *       / \           / \
 *    x A  NIL
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
 * PS: 
 * 从红黑树删除结点比二叉树多了几行代码. 多出的几行代码记录结点y的踪迹, 
 * y有可能导致红黑树性质破坏: 在结点被移除或者在树中移动之前, 必须记住y的颜色,
 * 并且记录结点x的踪迹, 将x移至树中y的原来位置, 因为结点x也可能引起红黑性质的破坏.
 */
inline
void tree_delete(tree_t* tree, tree_node_t* z)
{
    tree_node_t* x = NULL;
    tree_node_t* y = z;
    tree_node_color_t y_original_color = y->color;
	if (z->left == &tree->nil) {
        x = z->right;
		tree_transplant(tree, z, z->right);     // a)
    } else if (z->right == &tree->nil) {          
        x = z->left;
		tree_transplant(tree, z, z->left);      // b)
    } else {
		y = tree_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;      // x maybe nil
        } else {                // (y->parent != z)
			tree_transplant(tree, y, y->right); // d)
			y->right = z->right;                // d)
			y->right->parent = y;               // d)
		} 
		tree_transplant(tree, z, y);            // c)
		y->left = z->left;                      // c)
		y->left->parent = y;                    // c)
        y->color = z->color;                    // c)
	}
    if (y_original_color == kBlack)
        tree_delete_fixup(tree, x);
}

#ifdef __cplusplus
} // namespace red_black
} // namespace Hx
#endif // __cplusplus

#endif // MINI_STL_SET_IMPL_INC
