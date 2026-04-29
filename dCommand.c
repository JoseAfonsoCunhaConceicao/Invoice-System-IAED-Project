/**
 * @file dCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'd' command.
 * Allows deleting a specific invoice by ID or removing quantities of a 
 * specific product.
 */
#include "dCommand.h"



/**
 * @brief Deletes an invoice from the system and client history.
 */
static void deleteInvoice(List *invoices, int invoiceId, 
                           SystemContext *systemContext) {
    Invoice dummy = {invoiceId, 0, 0, "", NULL};
    Invoice *targetInvoice = (Invoice*) removeFromList(invoices, &dummy);
    if (!targetInvoice) {
        printf("%d: no such invoice\n", invoiceId);
        return;
    }
    Client *associatedClient = GET_INVOICE_CLIENT(targetInvoice);
    removeFromList(GET_CLIENT_INVOICE_LIST(associatedClient), targetInvoice);

    /** Output required metrics for deleted invoice */
    long long totalPrice = GET_INVOICE_TOTAL_VALUE(targetInvoice);
    printf("%lld.%02lld %s %s\n", totalPrice / 100, totalPrice % 100, 
           GET_INVOICE_NIF(targetInvoice), GET_CLIENT_NAME(associatedClient));
    
    /** Rollback system financial and unit counters for the active summary */
    systemContext->NumberOfSoldItems -= GET_INVOICE_TOTAL_ITEMS(targetInvoice);
    systemContext->TotalValue -= totalPrice;
    
    /** Do NOT decrement NumberOfInvoices (it represents emitted count) */
    free(targetInvoice);
}

/**
 * @brief Removes stock from a product or deletes it if stock reaches zero.
 */
static void removeProductStock(List *products, char *ean, int amount) {
    Product *product = findProductByEAN(products, ean);
    if (!product) {
        printf("%s: no such product\n", ean);
    } else if (GET_PRODUCT_AMOUNT_IN_BASKET(product) != NOT_IN_BASKET) {
        printf("product in use\n");
    } else if (amount <= 0 || amount > GET_PRODUCT_AMOUNT(product)) {
        printf("invalid quantity\n");
    } else {
        int newAmount = GET_PRODUCT_AMOUNT(product) - amount;
        printf("%d %s\n", newAmount, GET_PRODUCT_DESCRIPTION(product));
        if (newAmount == 0) {
            removeFromList(products, product);
            destroyProduct(product);
        } else SET_PRODUCT_AMOUNT(product, -amount);
    }
}
/**
 * @brief Executes the 'd' command, deleting invoices or removing unused 
 * products. Depending on the parsed arguments, either deletes an invoice from 
 * global memory and client history, or removes a specified quantity of 
 * available product stock.
 * @param products Pointer to the global list of system products.
 * @param invoices Pointer to the global list of system invoices.
 * @param systemContext Pointer to the global system context to update stats.
 */
void dCommand(List *products, List *invoices, SystemContext *systemContext) {
    int removeAmount;
    char *eanOrIdBuffer = allocateBuffer();
    if (isEndOfLine()) {
        free(eanOrIdBuffer);
        return;
    }
    scanf("%s", eanOrIdBuffer);
    if (isEndOfLine()) {
        /** 1 Arg provided: Assume it is an invoice ID for deletion */
        deleteInvoice(invoices, atoi(eanOrIdBuffer), systemContext);
    } else {
        /** 2 Args provided: Parse EAN and units to deduct product stock */
        scanf("%d", &removeAmount);
        if (!validateEAN(eanOrIdBuffer)) printf("invalid ean\n");
        else removeProductStock(products, eanOrIdBuffer, removeAmount);
        finishLine();
    }
    free(eanOrIdBuffer);
}
