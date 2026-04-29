/**
 * @file main.c
 * @author ist1118656 (Jose Conceicao)
 * @brief The main file of the IAED 25/26 Project.
 * @mainpage IAED 25/26 - Invoicing System
 * 
 * This project implements an invoicing management system for a store,
 * allowing efficient management of products, clients, and invoices.
 * It utilizes hybrid data structures (Doubly Linked Lists and AVL Trees)
 * to ensure O(log N) search performance and O(1) chronological insertions.
 * 
 * Main features:
 * - Product stock management (EAN-8/13).
 * - Processing of temporary shopping baskets.
 * - Issuance and management of invoices per client.
 * - Product search with wildcards.
 * - Global system statistics.
 */
#include "global.h"
#include "pCommand.h"
#include "lCommand.h"
#include "aCommand.h"
#include "rCommand.h"
#include "fCommand.h"
#include "cCommand.h"
#include "dCommand.h"

/**
 * @brief Initializes the IVA rates vector based on arguments or defaults.
 */
static void initIvaRates(int ivaRates[], int argc, char *argv[]) {
    for (int i = 0; i < IVA_VECTOR_SIZE; i++) ivaRates[i] = INVALID_IVA_RATE;
    if (argc > 1) {
        readIvaFile(ivaRates, argv[1]);
    } else {
        ivaRates['A' - 'A'] = 0;
        ivaRates['B' - 'A'] = 6;
        ivaRates['C' - 'A'] = 13;
        ivaRates['D' - 'A'] = 23;
    }
}

/**
 * @brief Dispatches the command to the appropriate handler.
 */
static void runCommand(char command, List *products, List *invoices, 
                       List *clients, List *basket, 
                       SystemContext *systemContext, int ivaRates[]) {
    switch (command) {
        case 'p': pCommand(products, ivaRates); break;
        case 'l': lCommand(products); break;
        case 'a': aCommand(products, basket); break;
        case 'r': rCommand(systemContext, products, ivaRates); break;
        case 'f': fCommand(invoices, clients, basket, systemContext); break;
        case 'c': cCommand(clients); break;
        case 'd': dCommand(products, invoices, systemContext); break;
    }
}

/**
 * @brief Reads the input from stdin and calls the respective command handler.
 * Controls the main lifecycle of the program, parsing the first character
 * of each line to determine the system state.
 * @param argc Number of command-line arguments.
 * @param argv Vector of strings containing the command-line arguments.
 * @return Returns 0 upon successful execution.
 */
int main (int argc, char *argv[]) {
    char command;
    /** Initialize data structure containers */
    List *products = createProductsList(), *invoices = createInvoicesList();
    List *clients = createClientsList(), *basket = createProductsList();
    
    /** System metadata state: {soldItems, totalProfit, invoiceCount, nextID} */
    SystemContext systemContext = {0, 0, 0, 1};
    int ivaRates[IVA_VECTOR_SIZE];

    /** Setup IVA rates and enter main command loop */
    initIvaRates(ivaRates, argc, argv);
    while (scanf(" %c", &command) == 1 && command != 'q') {
        runCommand(command, products, invoices, clients, basket, 
                   &systemContext, ivaRates);
    }

    /** Memory cleanup before exit */
    destroyList(clients, destroyClient);
    destroyList(invoices, destroyInvoice);
    destroyList(basket, NULL);
    destroyList(products, destroyProduct);
    return 0;
}