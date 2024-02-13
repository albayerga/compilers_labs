#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#define DEBUG 1 // 0 = off, 1 = on
#define OUTPUTFORMAT 1 // 0 = RELEASE (<lexeme, category> <lexeme, category> ...), 1 = DEBUG (line number <lexeme, category> <lexeme, category> ... \n)
#define OUTPUT_EXT "scn"
#define TOKEN_MAX 100000

// Function declarations
void saveToken(char tokenList[TOKEN_MAX][2][20]);
void formatTokenDebugMode(char tokenList[TOKEN_MAX][2][20]);
void formatTokenReleaseMode(char tokenList[TOKEN_MAX][2][20]);

#endif // SCANNER_H
