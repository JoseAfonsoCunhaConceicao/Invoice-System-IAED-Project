/**
 * @file global.h
 * @author ist1118656 (Jose Conceicao)
 * @brief Global definitions, constants, and generic function declarations
 * for the IAED 2025/2026 Project.
 */
#ifndef GLOBAL_H

#define GLOBAL_H

/** Needed libraries. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/** Definition of the utilized constants. */
#define MAX_SIZE_DESCRIPTION 50 /**< Max size for a product description. */
#define MAX_NUMBER_PRODUCTS 10000 /**< Max number of allowed products. */
#define MAX_EAN_SIZE 13 /**< Max size for an EAN code string. */
#define IVA_VECTOR_SIZE 26 /**< Max number of IVA categories (A-Z). */
#define INVALID_IVA_RATE -1 /**< Represents an unassigned IVA rate. */
#define SIZE_EAN_CODE_13 13 /**< Size of EAN-13 codes. */
#define SIZE_EAN_CODE_8 8 /**< Size of EAN-8 codes. */
#define NOT_FOUND -1 /**< Return code for item not found. */
#define FOUND 1 /**< Return code for item found. */
#define MAX_SIZE 65535 /**< Max buffer size for reading input lines. */
#define NIF_SIZE 9 /**< Size of the client NIF number. */
#define NOT_IN_BASKET 0 /**< Default amount of a product in the basket. */
#define VALID_DESCRIPTION 1 /**< Return code for valid descriptions. */
#define INVALID_DESCRIPTION 0 /**< Return code for invalid descriptions. */



/** Custom data types */
typedef struct listNode ListNode;
typedef struct invoice Invoice;
typedef struct client Client;

/** Comparison function type */
typedef int (*CompareFunc)(ListNode *, ListNode *);     

/** Generic Functions */
void handleNoMemory();
char* allocateBuffer();
int validateEAN(const char eanStr[]);
int isEndOfLine();
void readIvaFile(int listIva[], char *fileName);
int validateDescription (char description[]);
int readDescription(char description[]);
void finishLine ();
int match(char *wildCardPtr, char *eanStrPtr);
char* readClientName();


#endif