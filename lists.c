/**
 * @file lists.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of hybrid list operations (list + AVL tree).
 */
#include "lists.h"


/**
 * @brief Creates a new, empty list, optionally attaching a search tree.
 * @param tree Pointer to a previously created Tree for rapid lookup, or NULL.
 * @return A new dynamically allocated List instance.
 */
List* createList(Tree *tree) {
    List *list = malloc(sizeof(List));
    if (list == NULL) handleNoMemory();

    list -> size = 0;
    list -> head = NULL;
    list -> tail = NULL;
    list -> searchTree = tree;

    return list;
}

/**
 * @brief Allocates an internal node mapped to the generic item.
 * @param item The item pointer to store.
 * @return A newly allocated list node.
 */
ListNode* newListNode(void *item) {
    ListNode *node = malloc(sizeof(ListNode));
    if (node == NULL) handleNoMemory();

    node -> item = item;
    node -> next = NULL;
    node -> prev = NULL;

    return node;
}

/**
 * @brief Appends an item to the list (and into the tree if it exists).
 * @param list Pointer to the destination list.
 * @param item Pointer to the generic item to insert.
 */
void insertInList(List *list, void *item) {
    ListNode *newNode = newListNode(item);

    /** 1. Add node to the doubly linked list (O(1)) */
    if (list -> head == NULL) {
        list -> head = newNode;
        newNode -> prev = NULL;
    } else {
        newNode -> prev = list -> tail;
        list -> tail -> next = newNode;
    }

    list -> tail = newNode;
    list -> size += 1;

    /** 2. Mirror insertion into the AVL search tree if enabled (O(log N)) */
    if (list -> searchTree != NULL) insertInTree(list -> searchTree, newNode);

    return;
}

/**
 * @brief Unlinks a node from the doubly linked list.
 */
static void unlinkNode(List *list, ListNode *node) {
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else if (list->head == node) {
        list->head = node->next;
        node->next->prev = NULL;
    } else if (list->tail == node) {
        list->tail = node->prev;
        node->prev->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    list->size -= 1;
}

/**
 * @brief Removes an item from the list (and tree) without destroying it.
 * @param list Pointer to the target list.
 * @param item Pointer to an item matching the target to remove.
 * @return Pointer to the matched item that was just removed, or NULL.
 */
void* removeFromList(List *list, void *item) {
    ListNode dummy;
    dummy.item = item;
    /** 1. Fast lookup via tree structure */
    ListNode *node = searchInTree(list->searchTree, &dummy);
    if (!node) return NULL;

    /** 2. Unlink from temporal chronological sequence */
    unlinkNode(list, node);
    /** 3. Structural removal from AVL tree */
    removeFromTree(list->searchTree, node);

    void *foundItem = node->item;
    free(node);
    return foundItem;
}

/**
 * @brief Helper that clears nodes and optionally their items.
 */
static void freeAllNodes(List *list, DestroyFunc destroyItem) {
    ListNode *curr = list->tail;
    while (curr) {
        if (destroyItem) destroyItem(curr->item);
        ListNode *temp = curr;
        curr = curr->prev;
        free(temp);
    }
}

/**
 * @brief Destroys the entire list structure, its nodes, and tree.
 */
void destroyList(List *list, DestroyFunc destroyItem) {
    if (list->searchTree) destroyTree(list->searchTree);
    freeAllNodes(list, destroyItem);
    free(list);
}

/**
 * @brief Applies a callback function to every item in the list in 
 * chronological order.
 * @param list Pointer to the list to iterate through.
 * @param callback Function to execute for each item.
 * @param context Additional context parameter passed to the callback.
 */
void mapList(List *list, void (*callback)(void *item, void *context), 
             void *context) {
    ListNode *currentNode = list -> head;

    while (currentNode != NULL) {
        callback(currentNode -> item, context);

        currentNode = currentNode -> next;
    }
    return;
}

/**
 * @brief Internal wrapper holding both the item callback and its context.
 * Used exclusively for mapping in-order via the binary search tree.
 */
typedef struct {
    void (*callback)(void *item, void *context);
    void *context;
} MapInOrderContext;

/**
 * @brief Internal wrapper catching the tree's generic map function execution
 * to extract the item inner pointer from the tree's stored ListNode.
 * @param nodeItem The node being visited by the mapTree traversal.
 * @param context The custom wrapper context containing actual callback logic.
 */
static void mapInOrderWrapper(void *nodeItem, void *context) {
    ListNode *node = (ListNode*)nodeItem;
    MapInOrderContext *mapContext = (MapInOrderContext*)context;
    mapContext->callback(node->item, mapContext->context);
}

/**
 * @brief Applies a callback function to every item, ordered by the tree.
 * If no tree exists, this acts identically to mapList (chronological order).
 * @param list Pointer to the list containing the items.
 * @param callback Function to execute for each item.
 * @param context Additional context parameter passed to the callback.
 */
void mapListInOrder (List *list, void (*callback)(void *item, void *context), 
                     void *context) {
    Tree *tree = list -> searchTree;
    if (tree == NULL) {
        /** Fallback to linked list order if no tree exists */
        mapList(list, callback, context);
        return;
    }
    
    MapInOrderContext ctx;
    ctx.callback = callback;
    ctx.context = context;

    mapTree(tree, mapInOrderWrapper, &ctx);
    return;
}

/**
 * @brief Empties all elements from the list and its tree, but preserves 
 * the List structure itself.
 * Note: Does not free the memory allocated for the generic items.
 * @param list Pointer to the list to clear.
 */
void clearList(List *list) {

    if (list -> searchTree != NULL) {
        clearTreeNodes(GET_TREE_ROOT(list -> searchTree));
        SET_TREE_ROOT(list -> searchTree, NULL);
    }

    ListNode *tailNode = list -> tail, *prevNode;

    while(tailNode != NULL) {
        prevNode = tailNode -> prev;
        free(tailNode);
        tailNode = prevNode;
    }

    list -> size = 0;
    list -> head = NULL;
    list -> tail = NULL;

    return;
}

/**
 * @brief Searches for an item within the list.
 * Utilizes the associated Tree for O(log N) access if available, else 
 * O(N) list traversal.
 * @param list Pointer to the target list.
 * @param item Pointer to a dummy item containing search keys.
 * @return Pointer to the matched item, or NULL if not found.
 */
void* searchInList(List *list, void *item) {
    if (list == NULL) return NULL;
    Tree *tree = list -> searchTree;
    
    /** Option A: Linear search (O(N)) - used for the basket */
    if (tree == NULL) {
        ListNode *current = list -> head;
        while (current != NULL) {
            if (current -> item == item) return current -> item;
            current = current -> next;
        }
        return NULL;
    }

    /** Option B: Binary Tree search (O(log N)) - used for Product/Client */
    ListNode tempListNode;
    tempListNode.item = item;

    void *result = searchInTree(tree, &tempListNode);
    return (result == NULL) ? NULL : ((ListNode*)result) -> item;
}