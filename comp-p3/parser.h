#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>

#define ROWS 12
#define COLS 6
#define MAX_STACK 25
#define MAX_STRING 10
#define ACCEPT 1000

//declarar funciones
int getLength(char (*array)[COLS]);
int getTkn(char *token);
int parse(char *tokens[6], int size, FILE* outputFile);

#endif // PARSER_H