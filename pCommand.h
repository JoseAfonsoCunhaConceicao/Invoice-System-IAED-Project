/**
 * @file pCommand.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for the 'p' command (add product to system).
 */
#ifndef P_COMMAND_H

#define P_COMMAND_H

#include "products.h"

/**
 * @brief Parser component for the 'p' command.
 * Reads the line arguments for product creation and builds a new Product.
 * @param ivaRates Array of acceptable IVA percentages mapped by region.
 * @return Dynamically allocated partially setup Product, or NULL if invalid.
 */
Product* handleInputCommandP(const int ivaRates[]);

/**
 * @brief Executes the 'p' command, creating and adding a new product if valid.
 * Validates EAN length, unique constraint, price ranges, and IVA constraints.
 * @param products Pointer to the global list of system products.
 * @param ivaRates Array of tax rates mapped mimicking valid char ranges.
 */
void pCommand(List *products, const int ivaRates[]);

#endif