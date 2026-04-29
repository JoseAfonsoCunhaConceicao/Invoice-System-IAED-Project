/**
 * @file clients.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file defining the client data structure and operations.
 */
#ifndef CLIENTS_H

#define CLIENTS_H

#include "products.h"

/**
 * @brief Represents a client in the system.
 */
struct client{
    char *name;      /**< Dynamically allocated string for the client's name. */
    List *invoices;  /**< List containing all invoices billed to this client. */
};

/** Getters */
#define GET_CLIENT_NAME(c) (c -> name)
#define GET_CLIENT_INVOICE_LIST(c) (c -> invoices)

/** Setters */
#define SET_CLIENT_NAME(c, newName) ( free((c) -> name), \
        (c) -> name = (newName))
#define SET_CLIENT_INVOICE_LIST(c, newList) ((c) -> invoices = (newList))

/** 
 * @brief Macro to temporarily bind a string to a stack-allocated client 
 * for search purposes (avoids memory allocation).
 */
#define BIND_CLIENT_SEARCH_KEY(c, searchName) ((c)->name = (searchName))

/**
 * @brief Constructs a new client.
 * @param name The name of the client.
 * @param invoices Pointer to an existing list of invoices (or NULL).
 * @return A new allocated Client instance.
 */
Client* createClient(char *name, List *invoices);

/**
 * @brief Helper to find a client by name without manual dummy construction.
 * @param clients Pointer to the list of clients.
 * @param name Client name to search for.
 * @return Pointer to the client if found, NULL otherwise.
 */
Client* findClientByName(List *clients, char *name);

/**
 * @brief Compares two clients alphabetically by their name.
 * @param nodeA First list node containing a client.
 * @param nodeB Second list node containing a client.
 * @return > 0 if A > B, < 0 if A < B, 0 if equal.
 */
int compareClients(ListNode *nodeA, ListNode *nodeB);

/**
 * @brief Destroys a client instance and frees its associated memory.
 * @param item Pointer to the client to be destroyed.
 */
void destroyClient(void *item);

/**
 * @brief Creates a new abstract data type list configured for clients.
 * @return A pointer to the dynamically allocated list of clients.
 */
List* createClientsList ();

#endif