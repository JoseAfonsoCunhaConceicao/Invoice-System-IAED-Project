/**
 * @file rCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'r' command (remove product/calculate stats).
 */
#ifndef R_COMMAND_H

#define R_COMMAND_H

#include "invoices.h"

/**
 * @brief Executes the 'r' command.
 * Depending on arguments, either removes a product entirely from the system
 * or recalculates and corrects global system statistics based on product 
 * price/tax changes.
 * @param systemContext Pointer to the global system context to be 
 * correctly recalculated.
 * @param products Pointer to the global list of products.
 * @param ivaRates Array containing the current valid IVA mappings.
 */
void rCommand(SystemContext *systemContext, List *products, int ivaRates[]);


#endif