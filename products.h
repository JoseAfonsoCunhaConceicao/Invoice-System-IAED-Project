/**
 * @file products.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Header file defining the product data structure and its operations.
 */
#ifndef PRODUCT_H

#define PRODUCT_H

#include "lists.h"

/**
 * @brief Represents a product in the system.
 */
typedef struct {
    char eanStr[MAX_EAN_SIZE + 1];                  /**< EAN code string. */
    int iva;                                        /**< Applied IVA rate. */
    char ivaLetter;                                 /**< IVA letter code. */
    int price;                                      /**< Product price. */
    int amount;                                     /**< Available stock. */
    char description[MAX_SIZE_DESCRIPTION + 1];     /**< Description. */
    int inBasket;                                   /**< Units in basket. */
    int sold;                                       /**< Total units sold. */
} Product;

/**
 * @brief Allocates and initializes a new empty Product instance.
 * @return A dynamically allocated Product pointer.
 */
Product* createProduct ();

/**
 * @brief Compares two products chronologically (usually by creation sequence
 * inherently handled by tree insertion logic if unmodified).
 * @param nodeA First list node containing a product.
 * @param nodeB Second list node containing a product.
 * @return 0 to essentially allow duplicate keys or unordered behavior 
 * depending on the caller.
 */
int compareProducts(ListNode *nodeA, ListNode *nodeB);

/**
 * @brief Reset callback function applied to each product in the basket.
 * Rolls back the units from the basket to the available stock.
 * @param item Pointer to the product to reset.
 * @param context Additional context (unused).
 */
void resetBasket(void *item, void *context);

/** 
 * @brief Macro to completely clear tracking variables across all products
 * currenty in the given basket list, and then empties the list itself.
 */
#define CLEAR_BASKET(basket) \
    (mapList(basket, resetBasket, basket), clearList(basket))

/**
 * @brief Creates a new abstract data type list configured for products.
 * @return A pointer to the dynamically allocated list of products.
 */
List* createProductsList ();

/**
 * @brief Helper to find a product by EAN without manual dummy construction.
 * @param products Pointer to the list of products.
 * @param ean EAN code to search for.
 * @return Pointer to the product if found, NULL otherwise.
 */
Product* findProductByEAN(List *products, char *ean);

/**
 * @brief Destroys a product instance and frees its memory.
 * @param item Pointer to the product to be destroyed.
 */
void destroyProduct(void *item);

/** Getters */
#define GET_PRODUCT_EAN(product) ((product) -> eanStr)
#define GET_PRODUCT_IVA_RATE(product) ((product) -> iva)
#define GET_PRODUCT_IVA_LETTER(product) ((product) -> ivaLetter)
#define GET_PRODUCT_PRICE(product) ((product) -> price)
#define GET_PRODUCT_AMOUNT(product) ((product) -> amount)
#define GET_PRODUCT_DESCRIPTION(product) ((product) -> description)
#define GET_PRODUCT_AMOUNT_IN_BASKET(product) ((product) -> inBasket)
#define GET_PRODUCT_SOLD_UNITS(product) ((product) -> sold)

/** Setters */
#define SET_PRODUCT_EAN(product, ean) (strcpy((product) -> eanStr, ean))
#define SET_PRODUCT_IVA_RATE(product, ivaRate) ((product) -> iva = (ivaRate))
#define SET_PRODUCT_IVA_LETTER(product, newIvaLetter) \
    ((product) -> ivaLetter = (newIvaLetter))
#define SET_PRODUCT_PRICE(product, newPrice) ((product) -> price = (newPrice))
#define SET_PRODUCT_AMOUNT(product, quantity) \
    ((product) -> amount += (quantity))
#define SET_PRODUCT_DESCRIPTION(product, newDescription) \
        (strcpy((product) -> description, newDescription))
#define SET_PRODUCT_AMOUNT_IN_BASKET(product, quantity)\
        ((product) -> inBasket += quantity)
#define SET_PRODUCT_SOLD_UNITS(product, newSold) \
    ((product) -> sold += (newSold))


#endif