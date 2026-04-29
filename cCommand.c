/**
 * @file cCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'c' command for listing client invoices.
 */

#include "cCommand.h"

/**
 * @brief Callback function applied to print a single invoice.
 * @param item Pointer to the invoice to be printed.
 * @param context Additional context parameter (unused).
 */
void printInvoices (void *item, void *context) {
    /** Cast the pointer to the correct type */
    Invoice *invoice = (Invoice*) item;
    (void)context;

    int invoiceId = GET_INVOICE_ID(invoice);
    long long totalValue = GET_INVOICE_TOTAL_VALUE(invoice);
    char *clientName = GET_CLIENT_NAME(GET_INVOICE_CLIENT(invoice));

    printf("%d %lld.%02lld %s\n", invoiceId, totalValue / 100, 
           totalValue % 100, clientName);
    return;
}

/**
 * @brief Callback function applied to print all invoices belonging to a 
 * specific client.
 * Calls printInvoices on every element of the client's internal invoice list.
 * @param item Pointer to the client.
 * @param context Additional context parameter (unused).
 */
void printClientInvoices(void *item, void *context) {
    /** Cast the pointer to the correct type */
    Client *client = (Client*) item;

    List *invoices = GET_CLIENT_INVOICE_LIST(client);
    if (invoices == NULL) return;
    mapList(invoices, printInvoices, context);

    return;
}

/**
 * @brief Helper function to handle the case where no client name is provided.
 * @param clients Pointer to the list of clients in the system.
 */
static void handleAllClients(List *clients) {
    mapListInOrder(clients, printClientInvoices, NULL);
}

/**
 * @brief Searches for a client and prints their invoices.
 * @param clients Pointer to the system list of clients.
 * @param name Name of the client to search for.
 */
static void executeCCommand(List *clients, char *name) {
    /** Locate the client in the main tree structure */
    Client *client = findClientByName(clients, name);

    if (client != NULL) {
        /** Client found: Extract and print their linked transaction history */
        List* clientInvoices = GET_CLIENT_INVOICE_LIST(client);
        /** 
         * If all invoices were deleted, treat as if the client doesn't 
         * exist for this command 
         */
        if (GET_LIST_SIZE(clientInvoices) == 0) {
            printf("%s: no such client\n", name);
        } else {
            mapList(clientInvoices, printInvoices, NULL);
        }
    } else {
        printf("%s: no such client\n", name);
    }
}

/**
 * @brief Executes the 'c' command.
 * @param clients Pointer to the list of clients in the system.
 */
void cCommand(List *clients) {
    /** 1. Command invoked without arguments: List all system invoices */
    if (isEndOfLine()) {
        handleAllClients(clients);
        return;
    }
    
    /** 2. Specific client name provided: Parse and fetch their records */
    char *clientName = readClientName();
    if (clientName != NULL) {
        if (strcmp(clientName, "error") == 0 || clientName[0] == '\0') {
            finishLine();
        } else {
            executeCCommand(clients, clientName);
        }
        free(clientName);
    } else {
        finishLine();
    }
}
