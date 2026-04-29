/**
 * @file dCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'd' command (list invoices by product).
 */
#ifndef D_COMMAND_H

#define D_COMMAND_H

#include "invoices.h"

/**
 * @brief Executes the 'd' command, searching for all invoices containing a 
 * specific product. Expects an EAN code from standard input.
 * @param products Pointer to the global list of system products.
 * @param invoices Pointer to the global list of system invoices.
 * @param systemContext Pointer to the system context (unused but kept for 
 * signature consistency).
 */
void dCommand(List *products, List *invoices, SystemContext *systemContext);


#endif