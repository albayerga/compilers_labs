#ifndef STATES_H
#define STATES_H

#include <stdio.h>
#include <ctype.h>

// define categories
#define CAT_TYPES 37
#define CAT_KEYWORD 36
#define CAT_IDENTIFIER 38
#define CAT_LITERAL 39
#define CAT_NONESSENTIAL 40
#define CAT_NUMBER 41
#define CAT_SPECIALCHAR 42
#define CAT_OPERAND 43

// define states
// to do: matrix of states

// function declarations
int charToColumn(char c);

#endif // STATES_H