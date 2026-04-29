/**
 * @file rCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'r' command (system and product stats).
 * Displays either global system statistics + valid tax rates,
 * or detailed history elements for a specific product.
 */
#include "rCommand.h"


/**
 * @brief Prints global system statistics and valid tax rates.
 */
static void printSystemStats(SystemContext *systemContext, int ivaRates[]) {
    printf("%lld %d %lld.%02lld\n", systemContext->NumberOfSoldItems, 
           systemContext->NumberOfInvoices, 
           systemContext->TotalValue / 100, systemContext->TotalValue % 100);
    for (int i = 0; i < IVA_VECTOR_SIZE; i++) {
        if (ivaRates[i] != INVALID_IVA_RATE) {
            printf("%c %d%%\n", 'A' + i, ivaRates[i]);
        }
    }
}

/**
 * @brief Searches and prints stats for a specific product.
 */
static void printProductStats(List *products, char *ean) {
    Product *found = findProductByEAN(products, ean);

    if (found == NULL) {
        printf("%s: no such product\n", ean);
    } else {
        printf("%d %d %s\n", GET_PRODUCT_AMOUNT(found), 
               GET_PRODUCT_SOLD_UNITS(found), GET_PRODUCT_DESCRIPTION(found));
    }
}
/**
 * @brief Executes the 'r' command.
 */
void rCommand(SystemContext *systemContext, List *products, int ivaRates[]) {
    /** 1. Command without arguments: Total system status. */
    if (isEndOfLine()) {
        printSystemStats(systemContext, ivaRates);
        return;
    }
    
    /** 2. EAN provided: Retrieve and display detailed product history. */
    char *eanBuffer = allocateBuffer();
    scanf("%s", eanBuffer);
    if (!validateEAN(eanBuffer)) {
        printf("invalid ean\n");
    } else {
        printProductStats(products, eanBuffer);
    }
    free(eanBuffer);
    finishLine();
}