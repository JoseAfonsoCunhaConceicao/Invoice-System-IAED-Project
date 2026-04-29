/**
 * @file aCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'a' command (add product to basket).
 * Handles the logic for increasing/decreasing product quantities in the 
 * current active basket, verifying stock availability and constraints.
 */
#include "aCommand.h"


/**
 * @brief Helper function to find a product in a basket list by its EAN.
 * @param basket Pointer to the list acting as a basket.
 * @param ean EAN-8/13 code string to search for.
 * @return Pointer to the matched Product, or NULL if not found.
 */
static Product* findProductInBasket(List *basket, char *ean) {
    ListNode *current = GET_LIST_HEAD(basket);
    while (current) {
        Product *p = (Product*) GET_ITEM_ON_LIST_NODE(current);
        if (strcmp(GET_PRODUCT_EAN(p), ean) == 0) return p;
        current = GET_NEXT_LIST_NODE(current);
    }
    return NULL;
}

/**
 * @brief Helper function to validate if a product exists in the system.
 * @param products Pointer to the global products list.
 * @param ean EAN code string to search for.
 * @return Pointer to the product data, or NULL (with error printed).
 */
static Product* validateSystemProduct(List *products, char *ean) {
    Product *found = findProductByEAN(products, ean);
    if (!found) printf("%s: no such product\n", ean);
    return found;
}

/**
 * @brief Prints the formatted line for a product in the basket.
 * @param product Pointer to the product data.
 * @param newTotalAmount Total units that will be in the basket.
 */
static void printProductLine(Product *product, int newTotalAmount) {
    char ivaLetter = GET_PRODUCT_IVA_LETTER(product);
    int price = GET_PRODUCT_PRICE(product);
    char *description = GET_PRODUCT_DESCRIPTION(product);

    printf("%s %d %c %d.%02d %s\n", GET_PRODUCT_EAN(product), 
           newTotalAmount, ivaLetter, price / 100, price % 100, description);
}

/**
 * @brief Updates the product state and the basket list.
 * @param product Pointer to the product found in the system.
 * @param quantity Amount to add or remove.
 * @param basket Pointer to the system basket.
 */
static void updateProductAndBasket(Product *product, int quantity, 
                                   List *basket) {
    /** 1. Update internal amount in basket */
    int currentInBasket = GET_PRODUCT_AMOUNT_IN_BASKET(product);
    SET_PRODUCT_AMOUNT_IN_BASKET(product, currentInBasket + quantity);

    /** 2. Check if product should be added to or removed from the list */
    Product *inBasket = findProductInBasket(basket, GET_PRODUCT_EAN(product));
    
    if (inBasket == NULL && (currentInBasket + quantity) > 0) {
        /** Add new reference to the basket list */
        appendToList(basket, product);
    } else if (inBasket != NULL && (currentInBasket + quantity) == 0) {
        /** Remove reference from the basket list */
        removeFromList(basket, product);
    }
}

/**
 * @brief Performs the actual product addition/removal logic for aCommand.
 * @param product Pointer to the product matched in the system.
 * @param quantity Amount to add (positive) or remove (negative).
 * @param basket Pointer to the current system basket.
 */
static void executeACommand(Product *product, int quantity, List *basket) {
    int stockAmount = GET_PRODUCT_AMOUNT(product);
    int currentInBasket = GET_PRODUCT_AMOUNT_IN_BASKET(product);
    int finalAmountInBasket = currentInBasket + quantity;

    /** Validate operations against stock and basket limits */
    if (finalAmountInBasket < 0) {
        printf("invalid quantity\n");
        return;
    } else if (stockAmount - quantity < 0) {
        printf("no stock\n");
        return;
    }

    /** Print success state and commit changes */
    printProductLine(product, finalAmountInBasket);
    updateProductAndBasket(product, quantity, basket);
}

/**
 * @brief Helper to parse quantity and EAN from stdin.
 * @param quantity Pointer to store the parsed quantity.
 * @return Dynamically allocated EAN string (caller must free).
 */
static char* parseAArgs(int *quantity) {
    char *buffer = allocateBuffer();
    char *ean = malloc(MAX_EAN_LENGTH + 1);

    if (scanf("%s %d", ean, quantity) != 2) {
        *quantity = 1;
        /** Fallback: re-read only the EAN if quantity was omitted */
        strcpy(ean, buffer);
    }

    free(buffer);
    return ean;
}

/**
 * @brief Executes the 'a' command.
 */
void aCommand (List *products, List *basket) {
    int requestedQuantity = 1;
    char *eanBuffer = allocateBuffer();
    
    /** Read arguments and validate product existence */
    if (scanf("%s %d", eanBuffer, &requestedQuantity) != 2) {
        /** Case for single argument (just EAN, quantity defaults to 1) */
    }
    
    Product *found = validateSystemProduct(products, eanBuffer);
    if (found) {
        executeACommand(found, requestedQuantity, basket);
    }

    free(eanBuffer);
}
