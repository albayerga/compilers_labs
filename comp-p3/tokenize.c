#include "tokenize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void pushToken(StackOfTokens* stackOfTokens, Token token)
{
    if (stackOfTokens->top < MAX_TOKENS)
    {
        stackOfTokens->tokens[stackOfTokens->top++] = token;
    }
    else
    {
        printf("Stack overflow\n");
    }
}

Token popToken(StackOfTokens* stackOfTokens)
{
    return stackOfTokens->tokens[--stackOfTokens->top];
}


void tokenize(FILE* inputFile, FILE* outputFile, StackOfTokens* stackOfTokens)
{
    //read the tokens of the file and store them in a stack of tokens
    //token like <lexeme, category>

    Token token;
    char buffer[100];
    while (fscanf(inputFile, "%s", buffer) != EOF)
    {
        if (sscanf(buffer, "<%[^,],%[^>]>", token.lexeme, token.category) == 2)
        {
            pushToken(stackOfTokens, token);
            //write token.lexeme to output file
            fprintf(outputFile, "%s", token.lexeme);
        }
        else
        {
            printf("Invalid token format: %s\n", buffer);
        }
    }
}