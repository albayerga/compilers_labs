#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandling.h"
#include "scanner.h"
#include "states.h"

FILE* inputFile;
FILE* outputFile;

#if COUNTOP == ON && OUTPUTFORMAT == 1
    int op = 0;
    int totalOP = 0;

    struct CharIntPair {
        char ch[20];
        int num;
    };

    struct CharIntPair tokenOp[TOKEN_MAX];
#endif

int main(int argc, char**argv)
{
    if (argc < 2)
    {
        handleError(1, 1, "Usage: ./scanner <filename>");
        return 1;
    }

    char* inputFileName = argv[1];
    inputFile = fopen(inputFileName, "r");

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

    char tokenList[TOKEN_MAX][2][20];
    int token_index = 0;
    
    saveToken(tokenList);
    //depending on the outputformat, we call a function or another
    if(OUTPUTFORMAT == 1)
    {
        formatTokenDebugMode(tokenList);
    }
    else
    {
        formatTokenReleaseMode(tokenList);
    }

    fclose(inputFile);
    if (DEBUG == 1)
    {
        fclose(outputFile);
    }
    return 0;
}


//this function tokenises the code and stores it in a structure
void saveToken(char tokenList[TOKEN_MAX][2][20])
{
    char c;
    char ahead;
    char* word = malloc(100);
    int word_length = 0;
    int actualState = START_STATE;
    int token_index = 0;

    //read char by char
    while ((c = fgetc(inputFile)) != EOF)
    {
        int column = charToColumn(c);
        int sintaxError = notifySintaxErrorIfAny(column, c, outputFile);

        //in case we encounter any sintax error we will store it anyway (if we are in debug mode, otherwise we will just skip it and notify the user in the console)
        if (sintaxError == 1)
        {
            //skip to next character
            if(DEBUG == 1)
            {
                char errorString[20];
                sprintf(errorString, "INVALID CHAR: %c", c);
                strcpy(tokenList[token_index][0], errorString);
                strcpy(tokenList[token_index][1], "ERROR!!!");
                token_index++;
            }
            actualState = START_STATE;
            continue;
        }
        int nextState = StatesMatrix[actualState][column];

        // Add 8 for the 8 comparisons in the function isFinalState
        #if COUNTOP == ON && OUTPUTFORMAT == 1
            op = op + 8;
            totalOP = totalOP + 8;
        #endif
        
        //in case we encounter a final state we nned to know if the token is finished.
        //to do this we need to look ahead to the next character and compare it to the actual char
        if (isFinalState(nextState) == 1)
        {
            word[word_length] = c;
            word_length++;

            // Add 2 for the next comparisons CAT_NUMBER and CAT_IDENTIFIER
            #if COUNTOP == ON && OUTPUTFORMAT == 1
                op++;
                totalOP++;
            #endif

            if (nextState == CAT_NUMBER || nextState == CAT_IDENTIFIER)
            {
                //look ahead: if there is a number or identifier ahead, keep reading
                ahead = fgetc(inputFile);
                int column_ahead = charToColumn(ahead);
                int nextState_ahead = StatesMatrix[nextState][column_ahead];

                while (nextState_ahead == CAT_NUMBER || nextState_ahead == CAT_IDENTIFIER)
                {

                    // For each iteration we add 2 comparisons
                    #if COUNTOP == ON && OUTPUTFORMAT == 1
                        op = op + 2;
                        totalOP = totalOP + 2;
                    #endif

                    word[word_length] = ahead;
                    word_length++;
                    ahead = fgetc(inputFile);
                    column_ahead = charToColumn(ahead);
                    nextState_ahead = StatesMatrix[nextState][column_ahead];
                }
                ungetc(ahead, inputFile);
            }

            word[word_length] = '\0';

            //Save token to list
            strcpy(tokenList[token_index][0], word);
            strcpy(tokenList[token_index][1], finalStateToString(nextState));

            #if COUNTOP == ON && OUTPUTFORMAT == 1
                strcpy(tokenOp[token_index].ch, word);
                tokenOp[token_index].num = op;
                op = 0;
            #endif

            token_index++;

            word_length = 0;
            actualState = START_STATE;
        }

        //here we are not in a final state so we just keep reading
        else
        {
            word[word_length] = c;
            word_length++;
            actualState = nextState;
        }
    }
    free(word);
}


// Function to format and print tokens in debug mode
// Input: tokenList - array of tokens to be formatted [TOKEN_MAX][2][20]
void formatTokenDebugMode(char tokenList[TOKEN_MAX][2][20])
{
    int lineNum = 1; // Variable to store the current line number
    int lastWasLineJump = 0; // Flag to track if the last token was a line jump
    int isFirstToken = 1; // Flag to track if it's the first token on a line

    // Loop through the token list until an empty token is encountered
    for (int i = 0; tokenList[i][0][0] != '\0'; i++)
    {
        // Check if the token is a line jump
        if (strcmp(tokenList[i][0], "\n") == 0 && lastWasLineJump == 1)
        {
            lineNum++; // Increment line number to skip line
        }
        else if (strcmp(tokenList[i][0], "\n") == 0) // If it's a line jump
        {
            fprintf(outputFile, "\n\n"); // Print two line breaks
            lastWasLineJump = 1; // Set flag indicating the last token was a line jump
            lineNum++; // Increment line number
            isFirstToken = 1; // Reset flag for first token on a line
        }
        else // If it's not a line jump
        {
            // Control when to print line number
            if (isFirstToken == 1)
            {
                fprintf(outputFile, "%d ", lineNum); // Print line number if it's the first token on the line
            }
            fprintf(outputFile, "<%s, %s>", tokenList[i][0], tokenList[i][1]); // Print token

            #if COUNTOP == ON
                fprintf(outputFile, " %d ", tokenOp[i].num); // printing all number of operations
            #endif

            lastWasLineJump = 0; // Reset flag for line jump
            isFirstToken = 0; // Reset flag for first token on a line
        }
    }
    #if COUNTOP == ON
        fprintf(outputFile, "\n\nTotal number of operations: %d", totalOP); // print total number of operations at the end of the file
    #endif
}


// Function to format and print tokens in release mode
// Input: tokenList - array of tokens to be formatted [TOKEN_MAX][2][20]
void formatTokenReleaseMode(char tokenList[TOKEN_MAX][2][20])
{
    int lastWasLineJump = 0; // Flag to track if the last token was a line jump

    // Loop through the token list until an empty token is encountered
    for (int i = 0; tokenList[i][0][0] != '\0'; i++)
    {
        // Check if the token is a line jump and the last token was also a line jump
        if (strcmp(tokenList[i][0], "\n") == 0 && lastWasLineJump == 1)
        {
            // Do nothing, avoid printing consecutive line jumps
        }
        else if (strcmp(tokenList[i][0], "\n") == 0) // If it's a line jump
        {
            fprintf(outputFile, "\n"); // Print a single line break
            lastWasLineJump = 1; // Set flag indicating the last token was a line jump
        }
        else // If it's not a line jump
        {
            fprintf(outputFile, "<%s, %s>", tokenList[i][0], tokenList[i][1]); // Print token
            lastWasLineJump = 0; // Reset flag for line jump
        }
    }
}


