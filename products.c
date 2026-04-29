/**
 * @file products.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Implementation of product data structures and related operations.
 */
#include "products.h"


/**
 * @brief Allocates and initializes a new empty Product instance.
 * @return A dynamically allocated Product pointer.
 */
Product* createProduct () {
    Product *product = malloc(sizeof(Product));
    if (product == NULL) handleNoMemory();

    product -> sold = 0;
    product -> inBasket = NOT_IN_BASKET;
    product -> amount = 0;

    return product;
}


/**
 * @brief Compares two products by their unique EAN code.
 * @param nodeA First list node containing a product.
 * @param nodeB Second list node containing a product.
 * @return > 0 if A > B, < 0 if A < B, 0 if equal.
 */
int compareProducts(ListNode *nodeA, ListNode *nodeB) {
    /** Cast pointer types to the Product type */
    Product *productA = (Product *) GET_ITEM_ON_LIST_NODE(nodeA);
    Product *productB = (Product *) GET_ITEM_ON_LIST_NODE(nodeB);

    /** Get EAN Codes */
    char *eanCodeA = GET_PRODUCT_EAN(productA);
    char *eanCodeB = GET_PRODUCT_EAN(productB);

    return strcmp(eanCodeA, eanCodeB);
}

/**
 * @brief Destroys a product instance and frees its memory.
 * @param item Pointer to the product to be destroyed.
 */
void destroyProduct(void *item) {
    /** Cast the pointer to the correct type */
    Product *product = (Product*) item;

    free(product);

    return;
}

/**
 * @brief Reset callback function applied to each product in the basket.
 * Rolls back the units from the basket to the available stock.
 * @param item Pointer to the product to reset.
 * @param context Additional context (unused).
 */
void resetBasket(void *item, void *context) {
    Product *product = (Product *) item;
    (void)context;
    
    int inBasket = GET_PRODUCT_AMOUNT_IN_BASKET(product);

    SET_PRODUCT_AMOUNT_IN_BASKET(product, -inBasket);
    SET_PRODUCT_AMOUNT(product, inBasket);
    SET_PRODUCT_SOLD_UNITS(product, -inBasket);

    return;
}

/**
 * @brief Helper to find a product by EAN without manual dummy construction.
 */
Product* findProductByEAN(List *products, char *ean) {
    Product dummy;
    /** Bind key to local stack dummy to perform O(log N) lookup */
    SET_PRODUCT_EAN(&dummy, ean);
    return searchInList(products, &dummy);
}

/**
 * @brief Creates a new abstract data type list configured for products.
 * @return A pointer to the dynamically allocated list of products.
 */
List* createProductsList () {
    Tree *searchTree = createTree(compareProducts);

    return createList(searchTree);
}