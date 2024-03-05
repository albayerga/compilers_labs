#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// int getLength(char (*array)[COLS]){
//     return sizeof(*array) / sizeof((*array)[0]);
// }

// AUTOMATA PARAMS INITIALIZATION
char Stack[MAX_STACK][MAX_STRING]; // Stack
int actualState = 0;
int TOP = 0;
int tokens_index = 0;

int table[ROWS][COLS] = {{1, -1, -1, -1, -1, 2},
                         {-1, 0, -1, -1, 0, -1},
                         {-1, 3, -1, -1, ACCEPT, -1},
                         {-1, -1, 4, -1, -1, -1},
                         {5, -1, -1, -1, -1, 6},
                         {-1, 4, -1, 4, -1, -1},
                         {-1, 8, -1, 7, -1, -1},
                         {-1, 0, -1, -1, 0, -1},
                         {-1, -1, 9, -1, -1, -1},
                         {5, -1, -1, -1, -1, 10},
                         {-1, 8, -1, 11, -1, -1},
                         {-1, 4, -1, 4, -1, -1}};

// REDUCE AUX TABLE (positions to delete in stack)
int reduce_table[ROWS][COLS] = {{-1, -1, -1, -1, -1, -1},
                                {-1, 1, -1, -1, 1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, 1, -1, 1, -1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, 5, -1, -1, 5, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1},
                                {-1, 5, -1, 5, -1, -1}};

// char tokens[6][10] = {"int", "+", "(", "int", ")", "$"};

int getTkn(char *token){
    if(strcmp(token, "int") == 0){return 0;}
    if(strcmp(token, "+") == 0){return 1;}
    if(strcmp(token, "(") == 0){return 2;}
    if(strcmp(token, ")") == 0){return 3;}
    if(strcmp(token, "$") == 0){return 4;}
    if(strcmp(token, "E") == 0){return 5;}
    return -1; // Default return value
}

int parse(char *tokens[6], int size, FILE* outputFile)
{
    char operation[100];
    int previousState;
    int success = 0;
    int tokensLength = size;
    while(tokens_index < tokensLength){
        if(actualState == -1){
            fprintf(outputFile, "Syntax Error!!!\n");
            break;
        }

        // Read token
        char *newToken = tokens[tokens_index];
        tokens_index++;

        int tkn = getTkn(newToken);
        //fprintf(outputFile, "state: %d, token string: %s, token: %d\n", actualState, newToken, tkn);

        previousState = actualState;

        // Test if reduce
        if(reduce_table[actualState][tkn] != -1){
            // Then REDUCE
            strcpy(operation, "REDUCE");
            TOP = TOP - reduce_table[actualState][tkn];
            strcpy(Stack[TOP], "E"); // ONLY IN THAT AUTOMATA
            TOP++;
            tokens_index--;
            actualState = table[actualState][tkn];
            
            //GOTO
            if(actualState == 0){ actualState = 2; }
            if(actualState == 4){ actualState = 6; }
            if(actualState == 9){ actualState = 10; }
        }else{
            // Then SHIFT
            strcpy(operation, "SHIFT");
            strcpy(Stack[TOP], newToken);
            TOP++;
            actualState = table[actualState][tkn];
        }

        fprintf(outputFile, "Operation: %s, Input position: %d, Current state: %d, Previous state: %d \n", operation, tokens_index, actualState, previousState);
        fprintf(outputFile, "Stack: ");
        for(int i = 0; i < TOP; i++){
            fprintf(outputFile, "%s ", Stack[i]);
        }
        fprintf(outputFile, "\n\n");


        if(actualState == ACCEPT){
            fprintf(outputFile, "Code parsed successfully\n");
            printf("Code parsed successfully\n");
            success = 1;
        }
    }

    if(success == 0){
        fprintf(outputFile, "There is a syntax error in your code\n");
    }

    return 0;
}
