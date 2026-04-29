/**
 * @file clients.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of client data structures and related operations.
 */
#include "clients.h"


/**
 * @brief Constructs a new client.
 * @param name The name of the client.
 * @param invoices Pointer to an existing list of invoices (or NULL).
 * @return A new allocated Client instance.
 */
Client* createClient(char *name, List *invoices) {
    Client *client = calloc(1, sizeof(Client));
    if (client == NULL) handleNoMemory();

    SET_CLIENT_NAME(client, name);
    SET_CLIENT_INVOICE_LIST(client, invoices);

    return client;
}

/**
 * @brief Compares two clients alphabetically by their name.
 * @param nodeA First list node containing a client.
 * @param nodeB Second list node containing a client.
 * @return > 0 if A > B, < 0 if A < B, 0 if equal.
 */
int compareClients(ListNode *nodeA, ListNode *nodeB) {
    /** Cast pointer types to the Client type */
    Client *clientA = (Client *) GET_ITEM_ON_LIST_NODE(nodeA);
    Client *clientB = (Client *) GET_ITEM_ON_LIST_NODE(nodeB);

    /** Get client names */
    char *clientNameA = GET_CLIENT_NAME(clientA);
    char *clientNameB = GET_CLIENT_NAME(clientB);

    /** Compare Names */
    return strcmp(clientNameA, clientNameB);
}

/**
 * @brief Destroys a client instance and frees its associated memory.
 * @param item Pointer to the client to be destroyed.
 */
void destroyClient (void *item) {
    /** Cast the pointer to the correct type */
    Client *client = (Client*) item;
    
    free(client -> name);

    destroyList(client -> invoices, NULL);

    free(client);
}

/**
 * @brief Creates a new abstract data type list configured for clients.
 * @return A pointer to the dynamically allocated list of clients.
 */
List* createClientsList () {
    Tree *searchTree = createTree(compareClients);

    return createList(searchTree);
}

/**
 * @brief Helper to find a client by name without manual dummy construction.
 */
Client* findClientByName(List *clients, char *name) {
    Client dummy;
    /** Bind name to local stack dummy to perform O(log N) lookup */
    BIND_CLIENT_SEARCH_KEY(&dummy, name);
    return searchInList(clients, &dummy);
}