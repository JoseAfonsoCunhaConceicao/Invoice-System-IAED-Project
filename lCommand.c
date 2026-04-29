/**
 * @file lCommand.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of the 'l' command (list products/wildcard search).
 * Lists all products or filters them through a custom pattern matching 
 * algorithm.
 */
#include "lCommand.h"


/**
 * @brief Prints a single product's metadata in the required format.
 */
static void printFormattedProduct(Product *product, 
                                   WildcardContext *searchContext) {
    char *eanCode = GET_PRODUCT_EAN(product);
    char ivaLetter = GET_PRODUCT_IVA_LETTER(product);
    int price = GET_PRODUCT_PRICE(product);
    int sold = GET_PRODUCT_SOLD_UNITS(product);
    int amount = GET_PRODUCT_AMOUNT(product);
    char *description = GET_PRODUCT_DESCRIPTION(product);

    printf("%s %c %d.%02d %d %d %s\n", eanCode, ivaLetter, 
           price / 100, price % 100, sold, amount, description);
    searchContext->matchesFound++;
}

/**
 * @brief Wrapper for mapList to print available products.
 * @param item Pointer to the product.
 * @param context Pointer to WildcardContext.
 */
void printAvailableProducts(void *item, void *context) {
    Product *product = (Product*) item;
    if (GET_PRODUCT_AMOUNT(product) > 0) {
        printFormattedProduct(product, (WildcardContext*)context);
    }
}

/**
 * @brief Removes redundant sequential '*' characters from a wildcard string.
 * Reduces expressions like "***" to "*" for matching efficiency.
 * @param str The wildcard string to compress in-place.
 */
void compressWildcard(char *str) {
    int len = strlen(str), read, write;

    
    for(read = 0, write = read; read < len; read++) {
        /** Skip current '*' if the previous stored character is also '*' */
        if(str[read] == '*' && write > 0 && str[write - 1] == '*')
            ;
        else str[write++] = str[read];
    }

    str[write] = '\0';

    return;
}

/**
 * @brief Evaluates a product's description against a wildcard pattern.
 * @param item Pointer to the product to evaluate.
 * @param context Pointer to the WildcardContext.
 */
void printMatchProducts(void *item, void *context) {
    Product *product = (Product*) item;
    WildcardContext *searchContext = (WildcardContext*) context;
    char *eanCode = GET_PRODUCT_EAN(product);

    if (GET_PRODUCT_AMOUNT(product) > 0 && match(searchContext->wildcard, 
                                                  eanCode)) {
        printFormattedProduct(product, searchContext);
    }
}

/**
 * @brief Processes a single wildcard search term.
 */
static void processWildcardTerm(List *products, char *rawPattern, 
                                   WildcardContext *searchContext){
    /** 1. Prepare and compress wildcard */
    char *wildcard = malloc(strlen(rawPattern) + 1);
    if (!wildcard) return;
    strcpy(wildcard, rawPattern);
    compressWildcard(wildcard);

    /** 2. Evaluate query constraints */
    if (strcmp(wildcard, "*") == 0) {
        mapList(products, printAvailableProducts, searchContext);
    } else {
        searchContext->wildcard = wildcard;
        mapList(products, printMatchProducts, searchContext);
    }

    /** 3. Display error if matches were zero */
    if (!searchContext->matchesFound) {
        printf("%s: no such product\n", rawPattern);
    }
    
    /** Reset context for next possible term */
    searchContext->matchesFound = 0;
    free(wildcard);
}

/**
 * @brief Executes the 'l' command.
 * @param products Pointer to the list of all system products.
 */
void lCommand (List *products) {
    WildcardContext searchContext = {NULL, 0};
    
    /** 1. Command without arguments: Dump all system resources entirely. */
    if (isEndOfLine()) {
        mapList(products, printAvailableProducts, &searchContext);
        if (!searchContext.matchesFound) printf("*: no such product\n");
    } else {
        /** 2. Iteratively process multiple space-separated wildcards */
        char *patternBuffer = allocateBuffer();
        while (scanf("%s", patternBuffer) == 1) {
            processWildcardTerm(products, patternBuffer, &searchContext);
            if (isEndOfLine()) break;
        }
        free(patternBuffer);
    }
}