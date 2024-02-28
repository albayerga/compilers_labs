#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "parser.h"
#include "tokenize.h"

FILE* inputFile;
FILE* outputFile;
StackOfTokens* stackOfTokens;

int main(int argc, char**argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./parser <filename>");
        return 1;
    }

    char* inputFileName = argv[1];
    inputFile = fopen(inputFileName, "r");

    if(inputFile == NULL)
    {
        fprintf(stderr, "Error: File %s not found\n", inputFileName);
        return 1;
    }

    //delete input ext
    for(int i = strlen(inputFileName) - 1; i >= 0; i--)
    {
        if(inputFileName[i] == '.')
        {
            inputFileName[i] = '\0';
            break;
        }
    }

    //add output ext
    char* outputFileName = strcat(inputFileName, OUTPUT_EXT);
    outputFile = fopen(outputFileName, "w");

    //tokenize
    stackOfTokens = (StackOfTokens*)malloc(sizeof(StackOfTokens));
    stackOfTokens->top = 0;
    tokenize(inputFile, outputFile, stackOfTokens);

    //parse
    //to do

    fclose(inputFile);
    fclose(outputFile);
}