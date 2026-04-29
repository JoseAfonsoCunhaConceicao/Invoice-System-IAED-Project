/**
 * @file aCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'a' command (add product to basket).
 */
#ifndef A_COMMAND_H

#define A_COMMAND_H

#include "products.h"

/**
 * @brief Callback function applied to list all products currently in the 
 * basket.
 * @param item Pointer to the product to be printed.
 * @param context Additional context parameter (unused).
 */
void listAllProductsInBasket (void *item, void *context);

/**
 * @brief Executes the 'a' command, adding or removing product quantities from
 * the basket. Also handles listing the current basket if called without 
 * arguments.
 * @param products Pointer to the global list of system products.
 * @param basket Pointer to the current basket list.
 */
void aCommand (List *products, List *basket);

#endif