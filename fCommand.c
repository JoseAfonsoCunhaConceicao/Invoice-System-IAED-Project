/**
 * @file fCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'f' command (finalize invoice).
 * Concludes a sale by checking out the current basket, assigning products
 * to a client, calculating total sums, and generating an Invoice identifier.
 */
#include "fCommand.h"


/**
 * @brief Callback function to accumulate values from a product in the basket.
 * Accumulates cost according to unit price, IVA rates, and resets tracking.
 * @param item Pointer to the product.
 * @param context Pointer to the FinalizeContext tracking the totals.
 */
void finalizePurchase(void *item, void *context) {
    /** Cast the pointers to the correct type */
    Product *product = (Product*)(item);
    FinalizeContext *finalizeContext = (FinalizeContext*) context;

    int amount = GET_PRODUCT_AMOUNT_IN_BASKET(product);
    int price = GET_PRODUCT_PRICE(product);
    int ivaRate = GET_PRODUCT_IVA_RATE(product);

    finalizeContext -> totalValue += ((long long)amount * price * 
                                     (100 + ivaRate) + 50) / 100;
    finalizeContext -> totalProducts += amount;

    SET_PRODUCT_AMOUNT_IN_BASKET(product, -amount);

    return;
}



/**
 * @brief Helper function to retrieve an existing client or create a new one.
 * Uses a dummy element to execute an O(log N) tree lookup first list iteration.
 * @param clients Pointer to the list of clients.
 * @param name The name of the client to find or add.
 * @return Pointer to the matched/newly added client instance.
 */
Client* findOrAddClient(List *clients, char *name) {
    Client *client = findClientByName(clients, name);

    /** If client exists, return the client's pointer */
    if (client) {
        free(name); /** Avoid memory leak if client already exists */
        return client;
    }

    /** If not, create a new client */
    client = createClient(name, createInvoicesList());

    /** Insertion on the linked list and tree */
    insertInList(clients, client);

    return client;
}

/**
 * @brief Handles the actual invoice creation and system update.
 */
static void finalizeInvoice(List *invoices, Client *client, List *basket, 
                            SystemContext *systemContext, char *nif) {
    /** 1. Process basket items and calculate final pricing */
    FinalizeContext finalizeCtx = {0, 0};
    mapList(basket, finalizePurchase, &finalizeCtx);
    
    /** 2. Create the invoice registry */
    int invoiceId = systemContext->nextID++;
    Invoice *newInvoice = createInvoice(invoiceId, finalizeCtx.totalValue, 
                                        finalizeCtx.totalProducts, nif, client);
    
    /** 3. Bind invoice to system datasets */
    insertInList(invoices, newInvoice);
    insertInList(GET_CLIENT_INVOICE_LIST(client), newInvoice);
    clearList(basket);

    /** 4. Commit global statistics */
    systemContext->NumberOfInvoices++;
    systemContext->NumberOfSoldItems += finalizeCtx.totalProducts;
    systemContext->TotalValue += finalizeCtx.totalValue;

    /** 5. Output confirmation */
    printf("%lld %lld.%02lld %d\n", finalizeCtx.totalProducts, 
           finalizeCtx.totalValue / 100, finalizeCtx.totalValue % 100, 
           invoiceId);
}

/**
 * @brief Parses the client name if NIF was already provided or if starting 
 * with a name.
 */
static int parseNameArg(char **name, List *basket) {
    *name = readClientName();
    if (*name == NULL) return 0;

    if (strcmp(*name, "error") == 0) {
        CLEAR_BASKET(basket);
        finishLine();
        free(*name);
        *name = NULL;
        return 0;
    }

    if ((*name)[0] == '\0') {
        finishLine();
        free(*name);
        *name = NULL;
        return 0;
    }
    return 1;
}

/**
 * @brief Parses NIF and Client Name from stdin.
 * @return 1 on success, 0 on failure.
 */
static int parseFArgs(long *nifValue, char **name, List *basket) {
    if (isEndOfLine()) {
        *name = malloc(strlen("Cliente final") + 1);
        if (*name) strcpy(*name, "Cliente final");
        return (*name != NULL);
    } 
    
    char *token = allocateBuffer();
    if (scanf("%s", token) != 1) {
        free(token);
        return 0;
    }

    char *endptr;
    long val = strtol(token, &endptr, 10);

    /** If the entire token is a valid integer, treat it as a NIF */
    if (*endptr == '\0') {
        *nifValue = val;
        if (*nifValue < 100000000 || *nifValue > 999999999) {
            printf("%ld: no such nif\n", *nifValue);
            free(token);
            finishLine();
            return 0;
        }
        free(token);
        return parseNameArg(name, basket);
    } else {
        /** Not a valid integer (e.g., '8rui'), treat it as a Client Name */
        for (int i = (int)strlen(token) - 1; i >= 0; i--) {
            ungetc(token[i], stdin);
        }
        free(token);
        return parseNameArg(name, basket);
    }
}

/**
 * @brief Executes the 'f' command.
 * @param invoices Pointer to the list of invoices.
 * @param clients Pointer to the list of clients.
 * @param basket Pointer to the current active basket.
 * @param systemContext Pointer to the global system metrics structure.
 */
void fCommand (List *invoices, List *clients, List *basket, 
               SystemContext *systemContext) {
    long nifValue = 999999999;
    char *clientName = NULL, nifString[NIF_SIZE + 1];

    /** Check structural health (arguments validity and basket contents) */
    if (!parseFArgs(&nifValue, &clientName, basket)) {
        /** Ensure any partially read name buffer is freed */
        if (clientName) free(clientName);
        return;
    }

    /** Trigger invoice logic cascade */
    snprintf(nifString, sizeof(nifString), "%ld", nifValue);
    finalizeInvoice(invoices, findOrAddClient(clients, clientName), 
                    basket, systemContext, nifString);
}