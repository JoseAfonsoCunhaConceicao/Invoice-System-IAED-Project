/**
 * @file pCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'p' command (add product to system).
 * Handles the parsing and validation of new product entries or stock updates
 * for existing products.
 */
#include "pCommand.h"
/**
 * @brief Parses IVA letter and validates it against the rates.
 */
static int parseIva(const int IvaRates[], char *ivaLetter, int *ivaRateValue) {
    if (scanf(" %c", ivaLetter) != 1) return 0;
    
    /** Range check to avoid out-of-bounds access in IvaRates array */
    if (*ivaLetter < 'A' || *ivaLetter > 'Z') {
        printf("invalid iva\n");
        finishLine();
        return 0;
    }

    *ivaRateValue = IvaRates[*ivaLetter - 'A'];
    if (*ivaRateValue == INVALID_IVA_RATE) {
        printf("invalid iva\n");
        finishLine();
        return 0;
    }
    return 1;
}

/**
 * @brief Parses and validates price and quantity.
 */
static int parseFinancials(double *price, int *quantity) {
    if (scanf("%lf", price) != 1 || !(*price > 0)) {
        printf("invalid price\n");
        finishLine();
        return 0;
    }
    if (scanf("%d", quantity) != 1 || *quantity < 0) {
        printf("invalid quantity\n");
        finishLine();
        return 0;
    }
    return 1;
}

/**
 * @brief Parser component for the 'p' command.
 * Reads the line arguments for product creation and builds a new Product.
 * @param ean EAN code string.
 * @param ivaLetter Applied IVA letter.
 * @param ivaRate Applied IVA rate.
 * @param price Product price.
 * @param quantity Product quantity.
 * @param description Product description.
 * @return Dynamically allocated partially setup Product, or NULL if invalid.
 */
static Product* initProductFields(char *ean, char ivaLetter, int ivaRate, 
                                 double price, int quantity, 
                                 char *description) {
    Product *product = createProduct();
    SET_PRODUCT_EAN(product, ean);
    SET_PRODUCT_IVA_LETTER(product, ivaLetter);
    SET_PRODUCT_IVA_RATE(product, ivaRate);
    SET_PRODUCT_PRICE(product, (int)(price * 100.0 + 0.5));
    SET_PRODUCT_AMOUNT(product, quantity);
    SET_PRODUCT_DESCRIPTION(product, description);
    return product;
}

/**
 * @brief Parser component for the 'p' command.
 */
Product* handleInputCommandP (const int IvaRates[]) {
    char ean[MAX_EAN_SIZE+1], description[MAX_SIZE_DESCRIPTION+1], ivaLetter;
    double decodedPrice;
    int ivaRateValue, quantity;

    /** 1. Parse and validate EAN string */
    if (scanf("%s", ean) != 1) return NULL;
    if (!validateEAN(ean)) {
        printf("invalid ean\n");
        finishLine();
        return NULL;
    }

    /** 2. Parse and validate financial and metadata params */
    if (!parseIva(IvaRates, &ivaLetter, &ivaRateValue) || 
        !parseFinancials(&decodedPrice, &quantity) || 
        !readDescription(description)) return NULL;
    
    /** 3. Bundle parsed arguments into temporary Product structure */
    return initProductFields(ean, ivaLetter, ivaRateValue, decodedPrice, 
                             quantity, description);
}

/**
 * @brief Updates secondary fields of an existing product.
 */
static void updateProductMetadata(Product *current, Product *temp) {
    SET_PRODUCT_AMOUNT(current, GET_PRODUCT_AMOUNT(temp));
    SET_PRODUCT_DESCRIPTION(current, GET_PRODUCT_DESCRIPTION(temp));
    SET_PRODUCT_IVA_LETTER(current, GET_PRODUCT_IVA_LETTER(temp));
    SET_PRODUCT_IVA_RATE(current, GET_PRODUCT_IVA_RATE(temp));
}

/**
 * @brief Logic for updating an existing product's stock or data.
 */
static void updateExistingProduct(Product *current, Product *temp) {
    if (GET_PRODUCT_AMOUNT_IN_BASKET(current) != NOT_IN_BASKET) {
        /** Enforce immutability of prices if product is active in a basket */
        if (GET_PRODUCT_PRICE(current) != GET_PRODUCT_PRICE(temp)) {
            printf("product in use\n");
            destroyProduct(temp);
            return;
        }
        updateProductMetadata(current, temp);
    } else {
        /** Overwrite everything including price if unused */
        updateProductMetadata(current, temp);
        SET_PRODUCT_PRICE(current, GET_PRODUCT_PRICE(temp));
    }

    /** Print new total inventory count */
    printf("%d\n", GET_PRODUCT_AMOUNT(current));
    destroyProduct(temp);
}

/**
 * @brief Executes the 'p' command, creating and adding a new product if valid.
 * Validates EAN length, unique constraint, price ranges, and IVA constraints.
 * @param products Pointer to the global list of system products.
 * @param ivaRates Array of tax rates mapped by indexes mimicking char ranges.
 */
void pCommand (List *products, const int ivaRates[]) {
    /** Attempt extracting command parameters securely */
    Product *temp = handleInputCommandP(ivaRates);
    if (!temp) return;

    /** Cross-check system integrity and branch logic */
    Product *current = findProductByEAN(products, GET_PRODUCT_EAN(temp));
    if (current) {
        /** Extant Product: Update tracking values */
        updateExistingProduct(current, temp);
    } else {
        /** New Product: Ensure storage capacity limits and create */
        if (GET_LIST_SIZE(products) == MAX_NUMBER_PRODUCTS) {
            printf("invalid product\n");
            destroyProduct(temp);
            return;
        }
        insertInList(products, temp);
        printf("%d\n", GET_PRODUCT_AMOUNT(temp));
    }
}