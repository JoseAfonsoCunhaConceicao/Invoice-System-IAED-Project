/**
 * @file trees.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file defining the AVL tree structures and operations.
 */
#ifndef TREES_H

#define TREES_H


#include "global.h"

/**
 * @brief Represents a generic node in the AVL tree.
 */
typedef struct treeNode {
    void *item;                 /**< Pointer to the generic object. */
    struct treeNode *left;      /**< Pointer to the left child node. */
    struct treeNode *right;     /**< Pointer to the right child node. */
    int height;                 /**< Height of this node for AVL balancing. */
} TreeNode;

/**
 * @brief Represents the generic AVL tree container.
 */
typedef struct tree {
    TreeNode *origin;           /**< Pointer to the root node of the tree. */
    CompareFunc compare;        /**< Generic function for ordering nodes. */
} Tree;

/**
 * @brief Creates a new empty AVL search tree.
 * @param compare A pointer to the comparison function defining the ordering.
 * @return A dynamically allocated Tree instance.
 */
Tree* createTree(CompareFunc compare);

/** Getters */
#define GET_TREE_NODE_HEIGHT(node) (((node) == NULL) ? -1 : (node) -> height)
#define GET_BALANCE(node) (GET_TREE_NODE_HEIGHT((node)->right) \
        - GET_TREE_NODE_HEIGHT((node)->left))
#define GET_TREE_ROOT(tree) ((tree) -> origin)

/** Setters */
#define SET_TREE_ROOT(tree, newOrigin) ((tree) -> origin = (newOrigin))

/**
 * @brief Inserts a new item into the AVL tree, balancing it if necessary.

 * @param tree Pointer to the tree.
 * @param item Pointer to the generic item to insert.
 */
void insertInTree(Tree *tree, void *item);

/**
 * @brief Removes an item from the AVL tree and rebalances it.
 * @param tree Pointer to the tree.
 * @param item Dummy pointer carrying the search keys of the item to remove.
 */
void removeFromTree(Tree *tree, void *item);

/**
 * @brief Searches for an item in the AVL tree.
 * @param tree Pointer to the tree.
 * @param item Dummy pointer carrying the search keys.
 * @return Pointer to the matched item inside the tree, or NULL if not found.
 */
void* searchInTree(Tree *tree, void *item);

/**
 * @brief Recursively traverses the tree applying a callback function map.
 * @param tree Pointer to the tree.
 * @param callback Callback function to execute for each node's item.
 * @param context Additional context passed to the callback.
 */
void mapTree(Tree *tree, void (*callback)(void *item, void *context), 
             void *context);

/**
 * @brief Destroys all tree nodes, bypassing the list items memory itself.
 * @param node Root node of the tree to recursively clear.
 */
void clearTreeNodes (TreeNode *node);

/**
 * @brief Finds the node with the maximum value (rightmost leaf) originating 
 * from a subtree.
 * @param node Origin node of the subtree.
 * @return Pointer to the maximum node.
 */
TreeNode* getMax(TreeNode *node);

/**
 * @brief Destroys the tree structure and all its internal node wrappers 
 * without deleting the items.
 * @param tree Pointer to the tree.
 */
void destroyTree(Tree *tree);

#endif