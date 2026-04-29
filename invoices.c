/**
 * @file invoices.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of invoice data structures and operations.
 */
#include "invoices.h"


/**
 * @brief Constructs a new invoice.
 * @param id The unique identifier for the invoice.
 * @param totalValue The total cost in cents.
 * @param totalItems The total number of products.
 * @param nif The VAT identification number string.
 * @param clientRef Pointer to the client making the purchase.
 * @return A new allocated Invoice instance.
 */
Invoice* createInvoice(int id, long long totalValue, long long totalItems, 
                       char nif[], Client *clientRef) {
    Invoice *invoice = malloc(sizeof(Invoice));
    if (invoice == NULL) handleNoMemory();

    SET_INVOICE_ID(invoice, id);
    SET_INVOICE_TOTAL_VALUE(invoice, totalValue);
    SET_INVOICE_TOTAL_ITEMS(invoice, totalItems);
    SET_INVOICE_NIF(invoice, nif);
    SET_INVOICE_CLIENT(invoice, clientRef);

    return invoice;
}

/**
 * @brief Compares two invoices based on their unique ID.
 * @param nodeA First list node containing an invoice.
 * @param nodeB Second list node containing an invoice.
 * @return > 0 if A > B, < 0 if A < B, 0 if equal.
 */
int compareInvoices(ListNode *nodeA, ListNode *nodeB) {
    /** Cast pointer types to the Invoice type */
    Invoice *invoiceA = (Invoice *) GET_ITEM_ON_LIST_NODE(nodeA);
    Invoice *invoiceB = (Invoice *) GET_ITEM_ON_LIST_NODE(nodeB);

    /** Get invoice IDs */
    int invoiceIdA = GET_INVOICE_ID(invoiceA);
    int invoiceIdB = GET_INVOICE_ID(invoiceB);

    /** Invoices are unique by ID and assigned sequentially */
    return invoiceIdA - invoiceIdB;
}

/**
 * @brief Destroys an invoice instance and frees its memory.
 * @param item Pointer to the invoice to be destroyed.
 */
void destroyInvoice(void *item) {
    /** Cast the pointer to the correct type */
    Invoice *invoice = (Invoice*) item;

    free(invoice);
    return;
}

/**
 * @brief Creates a new abstract data type list configured for invoices.
 * @return A pointer to the dynamically allocated list of invoices.
 */
List* createInvoicesList () {
    Tree *searchTree = createTree(compareInvoices);

    return createList(searchTree);
}