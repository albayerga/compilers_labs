#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandling.h"
#include "scanner.h"
#include "states.h"

int main(int argc, char**argv)
{
    if (argc < 2)
    {
        handleError(1, 1, "Usage: ./scanner <filename>");
        return 1;
    }

    FILE* outputFile;
    char* inputFileName = argv[1];
    FILE* inputFile = fopen(inputFileName, "r");

    if (inputFile == NULL)
    {
        handleError(2, 1, inputFileName);
        return 1;
    }

    if(DEBUG == 1) //debug mode: file handler initialized to output file
    {
        strcat(inputFileName, OUTPUT_EXT);
        outputFile = fopen(inputFileName, "w");
    }
    else //release mode: file handler initialized to stdout
    {
        outputFile = stdout;
    }

    //read input file and extract tokens
    char c;
    char* word = malloc(100);
    int word_length = 0;
    int actualState = START_STATE;
    while((c = fgetc(inputFile)) != EOF)
    {
        int column = charToColumn(c);
        int nextState = StatesMatrix[actualState][column];
        
        if(isFinalState(nextState) == 1)
        {
            word[word_length] = c;
            word_length++;
            word[word_length] = '\0';
            fprintf(outputFile, "<%s, %s>", word, finalStateToString(nextState)); //to do: formatear token según outputformat
            word_length = 0;
            actualState = START_STATE;
            //to do: look ahead
        }
        else
        {
            word[word_length] = c;
            word_length++;
            actualState = nextState;
        }
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

