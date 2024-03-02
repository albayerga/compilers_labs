#include "tokenize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void pushToken(StackOfTokens* stackOfTokens, Token token)
{
    if (stackOfTokens->top < MAX_TOKENS)
    {
        stackOfTokens->top++;
        stackOfTokens->tokens[stackOfTokens->top] = token;
        stackOfTokens->size++;
    }
    else
    {
        printf("Stack overflow\n");
    }
}

Token popToken(StackOfTokens* stackOfTokens)
{
    stackOfTokens->top--;
    stackOfTokens->size--;
    return stackOfTokens->tokens[stackOfTokens->top];
}


void tokenize(FILE* inputFile, StackOfTokens* stackOfTokens)
{
    //read the tokens of the file and store them in a stack of tokens
    //token like <lexeme, category>

    char c;
    int i;

    while ((c = fgetc(inputFile)) != EOF)
    {
        if(c == '<')
        {
            //next is lexeme
            char lexeme[MAX_LEXEME_SIZE];
            i = 0;
            while ((c = fgetc(inputFile)) != ',')
            {
                lexeme[i] = c;
                i++;
            }
            lexeme[i] = '\0';

            //next is a space and then category, so i=1
            char category[MAX_CATEGORY_SIZE];
            i = 1;
            while ((c = fgetc(inputFile)) != '>')
            {
                category[i] = c;
                i++;
            }
            category[i] = '\0';

            //create token and push it to the stack
            Token token;
            strcpy(token.lexeme, lexeme);
            strcpy(token.category, category);
            pushToken(stackOfTokens, token);
        }
    }
}