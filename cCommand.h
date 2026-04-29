/**
 * @file cCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'c' command (list client invoices).
 */
#ifndef C_COMMAND_H

#define C_COMMAND_H

#include "invoices.h"

/**
 * @brief Callback function applied to print a single invoice.
 * @param item Pointer to the invoice to be printed.
 * @param context Additional context parameter (unused).
 */
void printInvoices (void *item, void *context);

/**
 * @brief Callback function applied to print all invoices belonging to a 
 * specific client.
 * @param item Pointer to the client.
 * @param context Additional context parameter (unused).
 */
void printClientInvoices(void *item, void *context);

/**
 * @brief Executes the 'c' command, listing all invoices for a client or 
 * all invoices in chronological order if no argument is provided.
 * @param clients Pointer to the list of clients in the system.
 */
void cCommand(List *clients);

#endif