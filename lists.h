/**
 * @file lists.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for doubly-linked list structures, optionally paired
 * with a binary search tree for fast lookup.
 */
#ifndef LISTS_H

#define LISTS_H

#include "trees.h"

/**
 * @brief Represents a node in the doubly-linked list.
 */
struct listNode {
    void *item;             /**< Pointer to the generic item stored. */
    struct listNode *prev;  /**< Pointer to the previous node. */
    struct listNode *next;  /**< Pointer to the next node. */
};

/**
 * @brief Represents a hybrid List structure.
 * Maintains chronological order using a doubly-linked list, while
 * optionally allowing fast O(log N) searches via an associated search tree.
 */
typedef struct {
    int size;               /**< Current number of elements in the list. */
    ListNode *head;         /**< Pointer to the first element (oldest). */
    ListNode *tail;         /**< Pointer to the last element (newest). */
    Tree *searchTree;       /**< Optional binary tree for fast item lookup. */
} List;

/** @brief Function pointer type for item destruction logic. */
typedef void (*DestroyFunc)(void*);

/** Getters */
#define GET_LIST_SIZE(list) ((list) -> size)
#define GET_ITEM_ON_LIST_NODE(node) ((node) -> item)

/** Setters */
#define SET_LIST_SIZE(list, newSize) ((list) -> size = (newSize))

/**
 * @brief Creates a new, empty list, optionally attaching a search tree.
 * @param tree Pointer to a previously created Tree to use for rapid lookup.
 * @return A new dynamically allocated List instance.
 */
List* createList(Tree *tree);

/**
 * @brief Appends an item to the list (and into the tree if it exists).
 * @param list Pointer to the destination list.
 * @param item Pointer to the generic item to insert.
 */
void insertInList(List *list, void *item);

/**
 * @brief Removes an item from the list (and tree) without destroying it.
 * @param list Pointer to the target list.
 * @param item Pointer to an item matching the target to remove.
 * @return Pointer to the matched item that was just removed, or NULL.
 */
void* removeFromList(List *list, void *item);

/**
 * @brief Searches for an item within the list.
 * Utilizes the associated Tree for O(log N) access if available, else 
 * O(N) list traversal.
 * @param list Pointer to the target list.
 * @param item Pointer to a dummy item containing search keys.
 * @return Pointer to the matched item, or NULL if not found.
 */
void* searchInList(List *list, void *item);

/**
 * @brief Empties all elements from the list and its tree, but preserves 
 * the List structure itself.
 * Note: Does not free the memory allocated for the generic items.
 * @param list Pointer to the list to clear.
 */
void clearList(List *list);

/**
 * @brief Applies a callback function to every item in the list in 
 * chronological order.
 * @param list Pointer to the list to iterate through.
 * @param callback Function to execute for each item.
 * @param context Additional context parameter passed to the callback.
 */
void mapList(List *list, void (*callback)(void *item, void *context), 
             void *context);

/**
 * @brief Applies a callback function to every item, ordered by the tree.
 * If no tree exists, this acts identically to mapList (chronological order).
 * @param list Pointer to the list containing the items.
 * @param callback Function to execute for each item.
 * @param context Additional context parameter passed to the callback.
 */
void mapListInOrder (List *list, void (*callback)(void *item, void *context), 
                     void *context);

/**
 * @brief Destroys the entire list structure, its nodes, and tree.
 * @param list Pointer to the list to destroy.
 * @param destroy Callback function to securely free each stored item's memory.
 */
void destroyList(List *list, DestroyFunc destroy);

#endif