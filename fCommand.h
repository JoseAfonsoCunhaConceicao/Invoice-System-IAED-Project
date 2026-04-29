/**
 * @file fCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'f' command (finalize invoice).
 */
#ifndef F_COMMAND_H

#define F_COMMAND_H

#include "invoices.h"

/**
 * @brief Context structure for calculating invoice totals dynamically.
 */
typedef struct { 
    long long totalValue;     /**< Accumulated total value calculation. */
    long long totalProducts;        /**< Accumulated count of total products. */
} FinalizeContext;

/**
 * @brief Callback function to calculate and accumulate values from a product
 * in the basket.
 * @param item Pointer to the product.
 * @param context Pointer to the FinalizeContext tracking the totals.
 */
void finalizePurchase(void *item, void *context);

/**
 * @brief Helper function to retrieve an existing client or create a new one.
 * @param clients Pointer to the list of clients.
 * @param name The name of the client to find or add.
 * @return Pointer to the matched/newly added client instance.
 */
Client* findOrAddClient(List *clients, char *name);

/**
 * @brief Executes the 'f' command, concluding a sale and creating a formal 
 * invoice. Parses the nif, client name, registers the sale on system stats, 
 * links it to the client, and empties the basket.
 * @param invoices Pointer to the list of invoices.
 * @param clients Pointer to the list of clients.
 * @param basket Pointer to the current active basket.
 * @param systemContext Pointer to the global system metrics structure.
 */
void fCommand (List *invoices, List *clients, List *basket, 
               SystemContext *systemContext);


#endif