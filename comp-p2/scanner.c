#include "ErrorHandling.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char**argv)
{
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
        FILE *outputFile = fopen(inputFileName, "w");
        // code here
        // output format: line number <lexeme, category> <lexeme, category> ... \n
        return 0;
    }
    else //release mode: file handler initialized to stdout
    {
        if (argc < 2)
        {
            handleError(1, 1, "Usage: ./scanner <filename>");
            return 1;
        }
        FILE *outputFile = stdout;
        // code here
        // output format: <lexeme, category> <lexeme, category> ...
        return 0;
    }
}


// DEFINE CATEGORIES LIKE CAT_CATNAME
// alphabet
// types: int char void
// operators: = > + *
// special chars: ( ) { } ; , [ ]
// numbers: 0-9
// keywords: if else main return while
// non essential: \n \t ' ' " "
// literal: "hello world"
// identifier: any alphanumeric string
//     doesn't start with a number
//     doesn't start with other category substrings, ex. can't start with i, c, v, e, m, r, w ?


//CONSTRUCT TOKENS
//token: line number <lexeme, category>


//READ FILE

