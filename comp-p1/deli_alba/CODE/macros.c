/*
 * Compilers
 * Practice 1: c pre-processor
 * File name: lib1.h
 * Functionalities of the library 1 (modify comment to be explicit
 * on what functionalities lib1 provides)
 * 
 * Define as many block functionalities (lib's) as the problem 
 * requires and also to assign a different block design and 
 * implementation to each person in the team
 * 
 * (Modify comments based on what you need this code to do)
 * Data structures and definitions defined by the lib1
 * 
 * Responsible: ROGER VIADER
 */
#include "./comp-p1.h"
#define MAX_WORD_SIZE 100
#define DICTIONARY_SIZE 100

char *takeWord1(FILE *inputFile, char* word, int maxLength)
{
    int c;
    int i = 0;

    if ((c = fgetc(inputFile)) != EOF && !isalpha(c) && c != '#') {
        word[i++] = c;
        word[i] = '\0';
        return 0;

    }

    while (c != EOF && (isalpha(c) || isdigit(c) || c == '#') && i < maxLength - 1) {
        word[i++] = c;
        c = fgetc(inputFile);
    }

    word[i] = '\0';
    if(c != EOF){
        fseek(inputFile, -1L, SEEK_CUR);
    }

    return 0;
}

//GLOBAL DICTIONARY
struct KeyValuePair
{
    char key[100];
    char attributes[100];
    char value[100];
};

struct KeyValuePair dictionary[1000];
int dictionarySize = 0;


//ATTRIBUTES DICTIONARY
struct StringList {
    char **strings;
    size_t size;
};

//this outputs a string of the attributes in the following format: (s1, s2, s3)
char* readAttributes(FILE* inputFile){

    char buffer[MAX_WORD_SIZE];
    int i = 0;
    int c;
    int numOpen= 0;
    int numClosed = 0;

    // Read characters until there is the same number of opening and closing parenthesis
    while ((c = fgetc(inputFile)) != EOF) {

        if (c == '(') numOpen++;
        if (c == ')') numClosed++;

        //when the parenthesis are opened and closed we stop reading

        if(numClosed == numOpen && numClosed > 0 && numOpen > 0)
        {
            
            buffer[i] = c;
            buffer[i+1] = '\0';

        }
        else
        {
            buffer[i] = c;
            i++;
        }
    }

    // Allocate memory for the word and copy the buffer
    char* word = malloc(i + 1);
    if (word == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    strcpy(word, buffer);
    //free(buffer);

    return word;

}

void addToDictionary(const char* key, const char* value, const char* attributes)
{
    if (dictionarySize < DICTIONARY_SIZE)
    {

        strcpy(dictionary[dictionarySize].key, key);
        strcpy(dictionary[dictionarySize].value, value);
        strcpy(dictionary[dictionarySize].attributes, attributes);

        dictionarySize++;
        
    }
    else
    {
        printf("Dictionary is full. Cannot add more entries.\n");
    }
}



void processMacro(FILE* inputFile, char* word) {

    char macroKey = *takeWord1(inputFile, word, sizeof(word));
    char macroAttributes = *readAttributes(inputFile);
    char macroValue = *readAttributes(inputFile);
    addToDictionary(&macroKey, &macroValue, &macroAttributes);
}



//to store the attributes in a struct
struct StringList parseString(const char *input) {
    struct StringList result;
    result.strings = NULL;
    result.size = 0;

    // Check if the input string starts and ends with '(' and ')'
    size_t len = strlen(input);
    if (len < 2 || input[0] != '(' || input[strlen(input) - 1] != ')') {
        fprintf(stderr, "Invalid input format\n");
        return result;
    }

    // Copy the input string to a temporary buffer to modify it
    char *tempInput = strdup(input);

    if (tempInput == NULL) {
        perror("Memory allocation failed");
        return result;
    }

    // Remove '(' at the beginning and ')' at the end
    tempInput[0] = '\0';
    tempInput[len - 1] = '\0';
    tempInput++;

    // Tokenize the string using ","
    char *token = strtok(tempInput, ",");
    while (token != NULL) {


        // Trim leading and trailing whitespaces from the token
        while (*token && (*token == ' ' || *token == '\t' || *token == '\n'))
            token++;
        size_t tokenLen = strlen(token);



        while (tokenLen > 0 && (token[tokenLen - 1] == ' ' || token[tokenLen - 1] == '\t' || token[tokenLen - 1] == '\n'))
            tokenLen--;



        // Allocate memory for the token and add it to the result list
        char *element = (char *)malloc((tokenLen + 1) * sizeof(char));

        if (element == NULL) {
            perror("Memory allocation failed");
            // Handle cleanup and return if needed
            return result;
        }

        strncpy(element, token, tokenLen);
        element[tokenLen] = '\0';

        result.strings = (char **)realloc(result.strings, (result.size + 1) * sizeof(char *));
        if (result.strings == NULL) {
            perror("Memory allocation failed");
            // Handle cleanup and return if needed
            free(element);
            return result;
        }

        result.strings[result.size] = element;
        result.size++;

        token = strtok(NULL, ",");
    }
}


//to free the struct that saves the attributes
void freeStringList(struct StringList *list) {
    for (size_t i = 0; i < list->size; ++i) {
        free(list->strings[i]); // free each string
    }
    free(list->strings); // free the array of strings
}



const char* searchKeyAndGetAttributes(const char* targetKey)
{
    for (int i = 0; i < dictionarySize; ++i)
    {
        if (strcmp(dictionary[i].key, targetKey) == 0)
        {
            // Key found, return its attributes
            return dictionary[i].attributes;
        }
    }

    // Key not found, return NULL
    return NULL;
}

char* searchKeyAndGetValue(const char* targetKey)
{
    for (int i = 0; i < dictionarySize; ++i)
    {
        if (strcmp(dictionary[i].key, targetKey) == 0)
        {
            // Key found, return its value
            return dictionary[i].value;
        }
    }

    // Key not found, return NULL
    return NULL;
}


//NECESSITEM UNA FUNCIÓ QUE SUBSTITUEIXI ELS ATTRIBUTES DE LA MACRO TROBADA AL VALUE DEL DICCIONARI


char* replaceSubstring(const char* original, const char* find, const char* replace) {
    // Check if the original string is NULL
    if (original == NULL) {
        return NULL;
    }

    // Find the substring in the original string
    char* start = strstr(original, find);

    // If the substring is not found, return the original string
    if (start == NULL) {
        return strdup(original);
    }

   
    // Allocate memory for the new string
    char* result = (char*)malloc(strlen(original) - strlen(find) + strlen(replace) + 1);

    // Copy the part of the original string before the found substring
    strncpy(result, original, start - original);

    // Null-terminate the result string
    result[start - original] = '\0';

    // Concatenate the replacement string
    strcat(result, replace);

    // Concatenate the part of the original string after the found substring
    strcat(result, start + strlen(find));

    return result;
}




// Function to get readattributes in value
char* getReadAttrInValue(char* macroName, char* readAttr)
{
    char* globalAttr = searchKeyAndGetAttributes(macroName);
    char* value = searchKeyAndGetValue(macroName);

    

    struct StringList structGlobalAttr = parseString(globalAttr);
    struct StringList structReadAttr = parseString(readAttr);


    // Iterate through each global attribute and replace in the value
    for(int i = 0; i < structGlobalAttr.size; i++){

        char* globalAttr = structGlobalAttr.strings[i];
        char* readAttr = structReadAttr.strings[i];

        // Use replaceSubstring function to replace occurrences in value
        value = replaceSubstring(value, globalAttr, readAttr);

    }


    // The modified value string is now stored in the 'value' variable
    return value;
}


