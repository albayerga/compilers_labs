#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandling.h"
#include "scanner.h"
#include "states.h"

FILE* inputFile;
FILE* outputFile;

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
    
    saveToken(tokenList);
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

void saveToken(char tokenList[TOKEN_MAX][2][20])
{
    char c;
    char ahead;
    char* word = malloc(100);
    int word_length = 0;
    int actualState = START_STATE;
    int token_index = 0;

    while ((c = fgetc(inputFile)) != EOF)
    {
        int column = charToColumn(c);
        int sintaxError = notifySintaxErrorIfAny(column, c, outputFile);
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
            continue;
        }
        int nextState = StatesMatrix[actualState][column];

        if (isFinalState(nextState) == 1)
        {
            word[word_length] = c;
            word_length++;

            if (nextState == CAT_NUMBER || nextState == CAT_IDENTIFIER)
            {
                //look ahead: if there is a number or identifier ahead, keep reading
                ahead = fgetc(inputFile);
                int column_ahead = charToColumn(ahead);
                int nextState_ahead = StatesMatrix[nextState][column_ahead];
                while (nextState_ahead == CAT_NUMBER || nextState_ahead == CAT_IDENTIFIER)
                {
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
            token_index++;

            word_length = 0;
            actualState = START_STATE;
        }
        else
        {
            word[word_length] = c;
            word_length++;
            actualState = nextState;
        }
    }
    free(word);
}

void formatTokenDebugMode(char tokenList[TOKEN_MAX][2][20])
{
    int lineNum = 1;
    int lastWasLineJump = 0;
    int isFirstToken = 1;
    for (int i = 0; tokenList[i][0][0] != '\0'; i++)
    {
        if (strcmp(tokenList[i][0], "\n") == 0 && lastWasLineJump == 1)
        {
            lineNum++; //Avoid line
        }
        else if (strcmp(tokenList[i][0], "\n") == 0)
        {
            fprintf(outputFile, "\n\n");
            lastWasLineJump = 1;
            lineNum++;
            isFirstToken = 1;
        }
        else
        {
            // Control when to print line number
            if (isFirstToken == 1)
            {
                fprintf(outputFile, "%d ", lineNum);
            }
            fprintf(outputFile, "<%s, %s>", tokenList[i][0], tokenList[i][1]);
            lastWasLineJump = 0;
            isFirstToken = 0;
        }
    }
}

void formatTokenReleaseMode(char tokenList[TOKEN_MAX][2][20])
{
    int lastWasLineJump = 0;
    for (int i = 0; tokenList[i][0][0] != '\0'; i++)
    {
        if (strcmp(tokenList[i][0], "\n") == 0 && lastWasLineJump == 1)
        {
            //Avoid line
        }
        else if (strcmp(tokenList[i][0], "\n") == 0)
        {
            fprintf(outputFile, "\n");
            lastWasLineJump = 1;
        }
        else
        {
            fprintf(outputFile, "<%s, %s>", tokenList[i][0], tokenList[i][1]);
            lastWasLineJump = 0;
        }
    }
}

