#include "ErrorHandling.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char**argv)
{
    FILE* outputFile;
    if(DEBUG == 1) //debug mode: file handler initialized to output file
    {
        if (argc < 2)
        {
            handleError(1, 1, "Usage: ./scanner <filename>");
            return 1;
        }
        char* inputFileName = argv[1];
        FILE* inputFile = fopen(inputFileName, "r");
        if (inputFile == NULL)
        {
            handleError(2, 1, inputFileName);
            return 1;
        }
        strcat(inputFileName, OUTPUT_EXT);
        outputFile = fopen(inputFileName, "w");
        return 0;
    }
    else //release mode: file handler initialized to stdout
    {
        if (argc < 2)
        {
            handleError(1, 1, "Usage: ./scanner <filename>");
            return 1;
        }
        outputFile = stdout;
        return 0;
    }

    if(OUTPUTFORMAT == 1)
    {
        //debug mode: extract tokens like: line number <lexeme, category> <lexeme, category> ... \n
        return 0;
    }
    else
    {
        //release mode: extract tokens like: <lexeme, category> <lexeme, category> ...
        return 0;
    }
}

