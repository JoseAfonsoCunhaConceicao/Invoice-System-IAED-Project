/**
 * @file lCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'l' command (list products/wildcard search).
 */
#ifndef L_COMMAND_H
#define L_COMMAND_H

#include "products.h"

/**
 * @brief Context structure for wildcard searches across products.
 */
typedef struct { 
    char *wildcard;         /**< The wildcard pattern to match against. */
    int matchesFound;       /**< Counter for total successful matches. */
} WildcardContext;

/**
 * @brief Prints a single product's metadata in the required format.
 * @param item Pointer to the product to print.
 * @param context Additional context parameter (unused).
 */
void printAvailableProducts(void *item, void *context);

/**
 * @brief Removes redundant sequential '*' characters from a wildcard string.
 * Reduces expressions like "***" to "*" for matching efficiency.
 * @param str The wildcard string to compress in-place.
 */
void compressWildcard(char *str);

/**
 * @brief Evaluates a product's description against a wildcard pattern and 
 * prints it on match. Uses the WildcardContext to extract the pattern and 
 * update the match counter.
 * @param item Pointer to the product to evaluate.
 * @param context Pointer to the WildcardContext.
 */
void printMatchProducts(void *item, void *context);

/**
 * @brief Executes the 'l' command.
 * Without arguments, lists all available products alphabetically.
 * With an argument, lists all products whose description matches the wildcard
 * pattern.
 * @param products Pointer to the list of all system products.
 */
void lCommand(List *products);


#endif