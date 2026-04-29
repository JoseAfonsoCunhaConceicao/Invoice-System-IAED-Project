/**
 * @file trees.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of AVL binary search tree logic including
 * self-balancing rotations, insertions, deletions, and mapping.
 */
#include "trees.h"

/** Forward declarations for internal recursive functions */
static TreeNode* auxRemoveFromTree(TreeNode *node, void* item, 
                                   CompareFunc compare);


/**
 * @brief Allocates and initializes a new tree node.
 * @param item Pointer to the generic item to store.
 * @return Dynamically allocated TreeNode.
 */
TreeNode* newTreeNode(void *item) {
    TreeNode *newNode = malloc(sizeof(TreeNode));
    if (newNode == NULL) handleNoMemory();

    newNode -> left = NULL;
    newNode -> right = NULL;
    newNode -> height = 0;
    newNode -> item = item;

    return newNode;
}

/**
 * @brief Creates a new empty AVL search tree.
 * @param compare A pointer to the comparison function defining the ordering.
 * @return A dynamically allocated Tree instance.
 */
Tree* createTree(CompareFunc compare) {
    Tree *tree = malloc(sizeof(Tree));
    if (tree == NULL) handleNoMemory();

    tree -> origin = NULL;
    tree -> compare = compare;

    return tree;
}

/**
 * @brief Finds the rightmost leaf of a subtree (the maximum value node).
 * @param node Root node of the considered subtree.
 * @return Pointer to the node with the highest value context.
 */
TreeNode* getMax(TreeNode *node) {
    while (node -> right != NULL) node = node -> right;

    return node;
}

/**
 * @brief Internal helper to return the maximum of two height integers.
 * @param left Left child node.
 * @param right Right child node.
 * @return The integer value of the highest height detected.
 */
static inline int max(TreeNode *left, TreeNode *right) {
    int heightA = GET_TREE_NODE_HEIGHT(left);
    int heightB = GET_TREE_NODE_HEIGHT(right);
    
    return heightA > heightB ? heightA : heightB;
}

/**
 * COMPLEX LOGIC EXPLANATION (AVL Rotations):
 * The following rotation functions (Left, Right, Right-Left, Left-Right)
 * are used to restore the O(log n) tree balance after insertions/deletions.
 * They execute a local restructuring of pointers to shift the subtree's
 * center of gravity without violating the binary search structural property
 * (Left children remain < Root, Right children remain > Root).
 */

/**
 * @brief Performs a simple left rotation.
 * @param node Root of the unbalanced structure.
 * @return Meaningful new root of the subtree after rotation.
 */
TreeNode* rotateLeft(TreeNode *node) {
    TreeNode *rightChild = node -> right;
    TreeNode *grandChild = rightChild -> left;
    
    /** 1. Perform rotation: pull right child up, push current node down */
    rightChild -> left = node;
    node -> right = grandChild;

    /** 2. Re-calculate heights for the affected nodes */
    node -> height = 1 + max(node -> left, node -> right);
    rightChild -> height = 1 + max(rightChild -> left, rightChild -> right);

    return rightChild;
}

/**
 * @brief Performs a simple right rotation.
 * @param node Root of the unbalanced structure.
 * @return Meaningful new root of the subtree after rotation.
 */
TreeNode* rotateRight(TreeNode *node) {
    TreeNode *leftChild = node -> left;
    TreeNode *grandChild = leftChild -> right;
    
    /** 1. Perform rotation: pull left child up, push current node down */
    leftChild -> right = node;
    node -> left = grandChild;

    /** 2. Re-calculate heights for the affected nodes */
    node -> height = 1 + max(node -> left, node -> right);
    leftChild -> height = 1 + max(leftChild -> left, leftChild -> right);

    return leftChild;
}

/**
 * @brief Performs a composite right-then-left rotation.
 * @param node Root of the unbalanced structure.
 * @return Meaningful new root of the subtree after rotations.
 */
TreeNode* rotateRightLeft(TreeNode *node) {
    node -> right = rotateRight(node -> right);
    
    return rotateLeft(node);
}

/**
 * @brief Performs a composite left-then-right rotation.
 * @param node Root of the unbalanced structure.
 * @return Meaningful new root of the subtree after rotations.
 */
TreeNode* rotateLeftRight(TreeNode *node) {
    node -> left = rotateLeft(node -> left);
    
    return rotateRight(node);
}

/**
 * @brief Checks subtree balance and performs required rotations if unbalanced.
 * Checks the balance factor (-1 to +1 is balanced). If magnitude > 1, applies
 * the required rotation based on the balance of the heavy child subtree.
 * @param node Subtree root to balance.
 * @return Subtree root properly balanced.
 */
TreeNode* balanceTree(TreeNode *node) {
    if (node == NULL) return NULL;

    /** 1. Refresh height to capture subtree changes */
    node -> height = 1 + max(node -> left, node -> right);

    int balance = GET_BALANCE(node);

    /** 2. Detect and correct Right-Heavy or Left-Heavy imbalances */
    if (balance < -1) {
        if (GET_BALANCE(node -> left) < 0) {
            node = rotateRight(node);
        } else node = rotateLeftRight(node);
    } else if (balance > 1) {
        if (GET_BALANCE(node -> right) > 0) {
            node = rotateLeft(node);
        } else node = rotateRightLeft(node);
    }

    return node;
}



/**
 * @brief Internal recursive function to insert a generic item into an AVL Node.
 * @param node Subtree root.
 * @param item Item to insert.
 * @param compare Comparison function.
 * @return Balancend subtree root.
 */
TreeNode* auxInsertInTree(TreeNode* node, void* item, CompareFunc compare) {
    /** Default insertion on a binary tree */
    if (node == NULL) {
        return newTreeNode(item);
    }

    int comparison = compare(item, node -> item);

    if (comparison < 0) {
        node -> left = auxInsertInTree(node -> left, item, compare);
    } else if (comparison > 0) {
        node -> right = auxInsertInTree(node -> right, item, compare);
    } else return node; /** Item already exists */
    
    /** Verify if the tree remains in balance */
    return balanceTree(node);
}

/**
 * @brief Inserts a new item into the AVL tree, balancing it if necessary.
 * @param tree Pointer to the tree.
 * @param item Pointer to the generic item to insert.
 */
void insertInTree(Tree *tree, void *item) {

    CompareFunc compare = tree -> compare;
    TreeNode *node = tree -> origin;

    tree -> origin = auxInsertInTree(node, item, compare);
}


/**
 * @brief Helper to handle the node removal logic once the node is found.
 */
static TreeNode* handleNodeRemoval(TreeNode *node, CompareFunc compare) {
    /** Case 1: Node has two children - find in-order predecessor */
    if (node->right != NULL && node->left != NULL) {
        TreeNode *successor = getMax(node->left);
        node->item = successor->item;
        node->left = auxRemoveFromTree(node->left, successor->item, compare);
        return balanceTree(node);
    }
    /** Option A: Linear search (O(N)) - used for the basket */
    TreeNode *childNode = (node->right != NULL) ? node->right : node->left;
    free(node);
    return childNode;
}

/**
 * @brief Internal recursive function to remove an item from an AVL Node.
 * Handles leaf nodes, nodes with one child, and nodes with two children
 * (by finding the in-order predecessor).
 * @param node Subtree root.
 * @param item Dummy pointer carrying the search keys of the item to remove.
 * @param compare Comparison function.
 * @return Balancend subtree root after deletion.
 */
static TreeNode* removeNode(TreeNode *node, void *item, CompareFunc compare) {
    int comparison = compare(item, node->item);
    if (comparison < 0) {
        node->left = auxRemoveFromTree(node->left, item, compare);
    } else if (comparison > 0) {
        node->right = auxRemoveFromTree(node->right, item, compare);
    } else {
        return handleNodeRemoval(node, compare);
    }
    return balanceTree(node);
}

/**
 * @brief Internal recursive helper for node removal from an AVL subtree.
 * Delegates the search and structural removal to removeNode.
 * @param node Subtree root.
 * @param item Item to remove.
 * @param compare Comparison function.
 * @return Balanced subtree root after removal.
 */
TreeNode* auxRemoveFromTree(TreeNode *node, void* item, CompareFunc compare) {
    if (node == NULL) return NULL;
    return removeNode(node, item, compare);
}

/**
 * @brief Removes an item from the AVL tree and rebalances it.
 * @param tree Pointer to the tree.
 * @param item Dummy pointer carrying the search keys of the item to remove.
 */
void removeFromTree(Tree *tree, void *item) {

    CompareFunc compare = tree -> compare;
    TreeNode *node = tree -> origin;

    tree -> origin = auxRemoveFromTree(node, item, compare);
}



/**
 * @brief Internal recursive function to search for an item in a subtree.
 * @param node Subtree root.
 * @param item Dummy pointer carrying search keys.
 * @param compare Comparison function.
 * @return Pointer to the matched TreeNode, or NULL if not found.
 */
TreeNode* auxSearchInTree(TreeNode *node, void *item, CompareFunc compare) {
    if (node == NULL) return NULL;
    
    int comparisonResult = compare(item, node -> item);

    if (comparisonResult < 0) {
        return auxSearchInTree(node -> left, item, compare);
    } else if (comparisonResult > 0) {
        return auxSearchInTree(node -> right, item, compare);
    } else return node; /** We have founded the node */
}

/**
 * @brief Searches for an item in the AVL tree.
 * @param tree Pointer to the tree.
 * @param item Dummy pointer carrying the search keys.
 * @return Pointer to the matched item inside the tree, or NULL if not found.
 */
void* searchInTree (Tree *tree, void *item) {

    if (tree == NULL) return NULL;
    CompareFunc compare = tree -> compare;
    TreeNode *node = tree -> origin;

    TreeNode *resultNode = auxSearchInTree(node, item, compare);
    
    return resultNode == NULL ? NULL : resultNode -> item;
}

/**
 * @brief Destroys all tree nodes, bypassing the list items memory itself.
 * @param node Root node of the tree to recursively clear.
 */
void clearTreeNodes (TreeNode *node) {

    if (node == NULL) return;

    clearTreeNodes(node -> left);
    clearTreeNodes(node -> right);
    free(node);

    return;
}

/**
 * @brief Internal recursive function to destroy nodes in post-order traversal.
 * @param node Current node to destroy.
 */
void auxDestroyTree(TreeNode *node) {
    if (node == NULL) return;
    
    auxDestroyTree(node -> left);
    auxDestroyTree(node -> right);

    free(node);
}

/**
 * @brief Destroys the tree structure and all its internal node wrappers 
 * without deleting the items.
 * @param tree Pointer to the tree.
 */
void destroyTree (Tree *tree) {

    TreeNode *node = tree -> origin;
    auxDestroyTree(node);
    free(tree);
}

/**
 * @brief Internal recursive function traversing the tree in-order to execute 
 * a callback.
 * @param node Root of the current subtree.
 * @param callback Callback function.
 * @param context Additional context parameter.
 */
void auxMapTree(TreeNode *node, 
                void (*callback)(void *item, void *context), void *context) {
    if (node == NULL) return;
    
    auxMapTree(node -> left, callback, context);

    callback(node -> item, context);

    auxMapTree(node -> right, callback, context);
}


/**
 * @brief Recursively traverses the tree applying a callback function map.
 * @param tree Pointer to the tree.
 * @param callback Callback function to execute for each node's item.
 * @param context Additional context passed to the callback.
 */
void mapTree(Tree *tree, void (*callback)(void *item, void *context), 
             void *context) {
    if (tree == NULL) return;

    auxMapTree(tree -> origin, callback, context);
}