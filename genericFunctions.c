/**
 * @file genericFunctions.c
 * @author ist1118656 (Jose Conceicao)
 * @brief Contains generic helper functions for validation, I/O processing,
 * pattern matching, and error handling.
 */
#include "global.h"


/**
 * @brief Prints a memory allocation error message and exits the program.
 */
void handleNoMemory() {
    printf("No memory.\n");
    exit(0);
}

/**
 * @brief Allocates a buffer of MAX_SIZE handling allocation failures.
 * @return Dynamically allocated buffer.
 */
char* allocateBuffer() {
    char *buffer = malloc(MAX_SIZE);
    if (!buffer) handleNoMemory();
    return buffer;
}

/**
 * @brief Validates an EAN-13 or EAN-8 code based on its check digit.
 * @param eanStr The EAN code string to be validated.
 * @return 1 if the EAN code is valid, 0 otherwise.
 */
int validateEAN(const char eanStr[]) {
   /** Size of the EAN code*/
   int length = strlen(eanStr);
   int i, weightedSum = 0, controlDigit, calculatedDigit;

   /** Validate the size of the code */
   if (length != SIZE_EAN_CODE_13 && length != SIZE_EAN_CODE_8) return 0;

   /** 1. Calculate weighted sum for check digit validation */
   for (i = 0; i < length - 1; i++) {
      int digit = eanStr[i] - '0';
      if (i % 2 == 0) weightedSum += digit;   /** even index: weight 1 */
      else weightedSum += 3 * digit;          /** odd index: weight 3 */
   }

   /** 2. Compare against the provided check digit */
   controlDigit = eanStr[length - 1] - '0';

   calculatedDigit = (10 - (weightedSum % 10)) % 10;

   return calculatedDigit == controlDigit;
}

/**
 * @brief Consumes trailing spaces and identifies if the end of line is reached.
 * Returns 1 if a newline character is the next relevant element. Otherwise,
 * it returns the character to the stream and returns 0.
 * @return 1 if at the end of the line, 0 otherwise.
 */
int isEndOfLine() {
   char nextChar;
   
   /** Reads all the ' ' before the next relevant element */
   while ((nextChar = getchar()) == ' ' || nextChar == '\t')
      ;
   if (nextChar == '\n') return 1;
   else {
      ungetc(nextChar, stdin);
      return 0;
   }
}

/**
 * @brief Reads the tax (IVA) configuration file and populates the given vector.
 * @param listIva Array mapping IVA letters (A-Z offsets) to tax rates.
 * @param fileName Name of the file to read the IVA configuration from.
 */
void readIvaFile(int listIva[], char *fileName) {
   
   FILE *fp = fopen(fileName, "r");
   /** Code letter */
   char ivaLetter;
   /** Rate of the tax */
   int ivaRateValue;

   if (fp == NULL) return; /** In case that the file doesn't open */

    while (fscanf(fp, " %c  %d", &ivaLetter, &ivaRateValue) == 2) {
        listIva[ivaLetter - 'A'] = ivaRateValue;
    }

   fclose(fp);
   
   return;
}

/**
 * @brief Validates if a product description starts with an uppercase letter.
 * Identifies standard ASCII and UTF-8 Portuguese uppercase characters.
 * @param description The product description to be validated.
 * @return VALID_DESCRIPTION if valid, INVALID_DESCRIPTION otherwise.
 */
int validateDescription (char description[]) {
   /** First character must be uppercase letter (ASCII A-Z or UTF-8 PT) */
   unsigned char firstByte = (unsigned char)description[0];
   int isUpper = 0;
   
   /** ASCII A-Z check */
   if (firstByte >= 'A' && firstByte <= 'Z') isUpper = 1;
   /** UTF-8 Multi-byte check (for accented characters like À, Á, etc.) */
   else if (firstByte == 0xC3) {
      unsigned char secondByte = (unsigned char)description[1];
      /** Portuguese uppercase accented characters in UTF-8: À-Þ(0x80-0x9E) */
      if (secondByte >= 0x80 && secondByte <= 0x9E) isUpper = 1;
   }

   if (!isUpper) {
      printf("invalid description\n");
      return INVALID_DESCRIPTION;
   }
   
   return VALID_DESCRIPTION;
}

/**
 * @brief Helper buffer reader for readDescription.
 * @param description Buffer to store the result.
 * @return 0 on success, -1 on failure/EOF.
 */
static int readDescriptionBuffer(char description[]) {
    int charIndex = 0, currentChar;
    /** 1. Consume all leading whitespace (including spaces and tabs) */
    while ((currentChar = getchar()) == ' ' || currentChar == '\t'); 
    
    /** If line ends immediately, it is an invalid description */
    if (currentChar == '\n' || currentChar == EOF) return -1;
    
    /** Store the first non-space character */
    description[charIndex++] = currentChar;

    /** 
     * 2. Read until end of line while enforcing character limits 
     * (max 50 chars) 
     */
    while ((currentChar = getchar()) != '\n' && currentChar != EOF) {
        if (charIndex < MAX_SIZE_DESCRIPTION) {
            description[charIndex++] = currentChar;
        } else {
            /** Increment to track if we exceeded the limit */
            charIndex++;
        }
    }
    
    /** Fail if more than MAX_SIZE_DESCRIPTION characters were provided */
    if (charIndex > MAX_SIZE_DESCRIPTION) return -1;
    
    description[charIndex] = '\0';
    return 0;
}

/**
 * @brief Reads a product description from standard input enforcing limits.
 * @param description Buffer to store the validated description.
 * @return VALID_DESCRIPTION if successful, INVALID_DESCRIPTION otherwise.
 */
int readDescription(char description[]) {
    if (readDescriptionBuffer(description) == -1) {
        printf("invalid description\n");
        return INVALID_DESCRIPTION;
    }
    return validateDescription(description);
}

/**
 * @brief Discards all remaining characters in the current line from stdin.
 */
void finishLine () {
    int nextChar;
    while ((nextChar = getchar()) != '\n' && nextChar != EOF);
    return;
}

/**
 * @brief Handles recursive branching for the '*' wildcard.
 */
static int matchWildcard(char *wild, char *ean) {
    /** 
     * Handles the '*' wildcard recursively:
     * 1. Assume '*' matches zero characters (move wild forward, keep ean)
     * 2. Assume '*' matches one or more characters (keep wild, move ean 
     * forward)
     */
    return match(wild + 1, ean) || (*ean != '\0' && match(wild, ean + 1));
}

/**
 * @brief Recursively matches an EAN string against a wildcard pattern.
 */
int match(char *wildCardPtr, char *eanStrPtr) {
    /** Both strings reached the end successfully */
    if (*wildCardPtr == '\0' && *eanStrPtr == '\0') return 1;

    /** Exact character match: advance both pointers */
    if (*wildCardPtr == *eanStrPtr) {
        return match(wildCardPtr + 1, eanStrPtr + 1);
    }

    /** Wildcard '*': Delegate to branching logic */
    if (*wildCardPtr == '*') return matchWildcard(wildCardPtr, eanStrPtr);

    /** Wildcard '?': Matches exactly one character (if not at EOF) */
    if (*wildCardPtr == '?') {
        if (*eanStrPtr == '\0') return 0;
        return match(wildCardPtr + 1, eanStrPtr + 1);
    }

    return 0;
}

/**
 * @brief Helper for readClientName.
 */
static char* createSignalString(char *val) {
    char *res = malloc(strlen(val) + 1);
    if (res) strcpy(res, val);
    return res;
}

/**
 * @brief Reads the client name from stdin.
 */
static int readRawName(char *nameBuffer, int *indexPtr, char currentChar) {
    if (currentChar == '"') {
        int foundClosingQuote = 0;
        while ((currentChar = getchar()) != EOF && currentChar != '\n') {
            if (currentChar == '"') {
                foundClosingQuote = 1;
                break;
            }
            if (*indexPtr < MAX_SIZE - 1) {
                nameBuffer[(*indexPtr)++] = currentChar;
            }
        }
        if (!foundClosingQuote) {
            /** Quote was never closed: force invalid name check */
             nameBuffer[0] = '1'; 
             *indexPtr = 1;
        }
    } else {
        nameBuffer[(*indexPtr)++] = currentChar;
        while ((currentChar = getchar()) != ' ' && currentChar != '\t' && 
               currentChar != '\n' && currentChar != EOF) {
            if (*indexPtr < MAX_SIZE - 1) {
                nameBuffer[(*indexPtr)++] = currentChar;
            }
        }
        if (currentChar != EOF) ungetc(currentChar, stdin);
    }
    nameBuffer[*indexPtr] = '\0';
    return *indexPtr;
}

/**
 * @brief Reads the client name from stdin.
 */
char* readClientName() {
    char nameBuffer[MAX_SIZE], currentChar = getchar();
    int charIndex = 0;
    
    /** 1. Consume leading spaces and tabs robustly */
    while (currentChar == ' ' || currentChar == '\t') {
        currentChar = getchar();
    }
    
    /** 
     * If we find a newline or EOF before the name starts, it's an 
     * empty name 
     */
    if (currentChar == '\n' || currentChar == EOF) return NULL;

    /** 2. Differentiate between quoted and unquoted names */
    readRawName(nameBuffer, &charIndex, currentChar);

    /** 3. Validate name format: must start with a letter (enunciado line 38) */
    if (charIndex == 0 || !isalpha((unsigned char)nameBuffer[0])) {
        if (charIndex > 0) printf("invalid name\n");
        return (charIndex > 0) ? createSignalString("") : NULL;
    }
    
    return createSignalString(nameBuffer);
}