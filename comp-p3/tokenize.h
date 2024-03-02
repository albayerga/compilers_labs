#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <stdio.h>
#define MAX_LEXEME_SIZE 100
#define MAX_CATEGORY_SIZE 100
#define MAX_TOKENS 1000

typedef struct{
    char lexeme[MAX_LEXEME_SIZE];
    char category[MAX_CATEGORY_SIZE];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int size;
    int top;
} StackOfTokens;

void pushToken(StackOfTokens* stackOfTokens, Token token);
Token popToken(StackOfTokens* stackOfTokens);
void tokenize(FILE* inputFile, StackOfTokens* stackOfTokens);

#endif // TOKENIZE_H