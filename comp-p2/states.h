#ifndef STATES_H
#define STATES_H

#include <stdio.h>
#include <ctype.h>

// define categories
#define CAT_TYPES 14
#define CAT_KEYWORD 11
#define CAT_IDENTIFIER 25
#define CAT_LITERAL 27
#define CAT_NONESSENTIAL 28
#define CAT_NUMBER 29
#define CAT_SPECIALCHAR 30
#define CAT_OPERAND 31

#define START_STATE 0
#define NUM_STATES 32
#define NUM_ALPHABET 24
#define INVALID_CHAR 300 //for example

extern int StatesMatrix[NUM_STATES][NUM_ALPHABET];

// function declarations
int charToColumn(char c);
int isFinalState(int state);
char* finalStateToString(int state);
int notifySintaxErrorIfAny(int column, char c, FILE* outputFile);

#endif // STATES_H