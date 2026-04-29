/**
 * @file invoices.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file for invoice data structures and operations.
 */
#ifndef INVOICES_H

#define INVOICES_H

#include "clients.h"

/**
 * @brief Represents an individual invoice.
 */
struct invoice {
  int id;                  /**< Unique sequential invoice identifier. */
  long long totalValue;    /**< Total value billed in this invoice in cents. */
  long long totalItems;    /**< Total number of items purchased. */
  char nif[NIF_SIZE + 1];  /**< The tax identification number of the client. */
  Client *clientRef;       /**< Pointer to the associated client structure. */
};

/**
 * @brief Global system context storing aggregate statistics.
 */
typedef struct systemContext{
  long long NumberOfSoldItems;    /**< Global total of items sold. */
  long long TotalValue;     /**< Global total value billed across invoices. */
  int NumberOfInvoices;     /**< Total number of persistent invoices emitted. */
  int nextID;               /**< Next sequential invoice ID to be assigned. */
} SystemContext;

/** Getters */
#define GET_INVOICE_ID(i) (i -> id)
#define GET_INVOICE_TOTAL_VALUE(i) (i -> totalValue)
#define GET_INVOICE_TOTAL_ITEMS(i) (i -> totalItems)
#define GET_INVOICE_NIF(i) (i -> nif)
#define GET_INVOICE_CLIENT(i) (i -> clientRef)

/** Setters */
#define SET_INVOICE_ID(i, number) (i -> id = number)
#define SET_INVOICE_TOTAL_VALUE(i, value) (i -> totalValue = value)
#define SET_INVOICE_TOTAL_ITEMS(i, nItems) (i -> totalItems = nItems)
#define SET_INVOICE_NIF(i, newNif) (strcpy(i -> nif, newNif))
#define SET_INVOICE_CLIENT(i, newClient) (i -> clientRef = newClient)

/**
 * @brief Destroys an invoice instance and frees its memory.
 * @param item Pointer to the invoice to be destroyed.
 */
void destroyInvoice(void * item);

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
                       char nif[], Client *clientRef);

/**
 * @brief Compares two invoices based on their unique ID.
 * @param nodeA First list node containing an invoice.
 * @param nodeB Second list node containing an invoice.
 * @return > 0 if A > B, < 0 if A < B, 0 if equal.
 */
int compareInvoices(ListNode *nodeA, ListNode *nodeB);

/**
 * @brief Creates a new abstract data type list configured for invoices.
 * @return A pointer to the dynamically allocated list of invoices.
 */
List* createInvoicesList ();

#endif