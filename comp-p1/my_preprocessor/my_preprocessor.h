#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

// Structure to hold flags
struct Flags
{
    int processDirectives;
    int processInclude;
    int processIfDef;
    int processComments;
    // Add other flags as needed
};

// Structure to hold dictionary entries
struct DictionaryEntry
{
    const char *key;
    const char *value;
};

// Function declarations
void printManPage();
char *takeWord(FILE *inputFile, long *filePosition);

void processFile(FILE *inputFile, FILE *outputFile, long *filePosition);

void processInclude(FILE *inputFile, FILE *outputFile, const char *filename);
void processDefine(FILE *inputFile, FILE *outputFile);
void processIfDef(FILE *inputFile, FILE *outputFile);
void processComments(FILE *inputFile, FILE *outputFile);

#endif // PREPROCESSOR_H