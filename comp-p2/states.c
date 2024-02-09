#include "states.h"

int charToColumn(char c)
{
    if(c == 'a')
    {
        return 0;
    }
    else if (c == 'c')
    {
        return 1;
    }
    else if(c == 'd')
    {
        return 2;
    }
    else if(c == 'e')
    {
        return 3;
    }
    else if(c == 'f')
    {
        return 4;
    }
    else if(c == 'h')
    {
        return 5;
    }
    else if(c == 'i')
    {
        return 6;
    }
    else if(c == 'l')
    {
        return 7;
    }
    else if(c == 'm')
    {
        return 8;
    }
    else if(c == 'n')
    {
        return 9;
    }
    else if(c == 'o')
    {
        return 10;
    }
    else if(c == 'r')
    {
        return 11;
    }
    else if(c == 's')
    {
        return 12;
    }
    else if(c == 't')
    {
        return 13;
    }
    else if(c == 'u')
    {
        return 14;
    }
    else if(c == 'v')
    {
        return 15;
    }
    else if(c == 'w')
    {
        return 16;
    }
    // s0 = b, g, j, k, p, q, x, y, z
    else if(c == 'b' || c == 'g' || c == 'j' || c == 'k' || c == 'p' || c == 'q' || c == 'x' || c == 'y' || c == 'z')
    {
        return 17;
    }
    // 0 - 9
    else if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
    {
        return 18;
    }
    // A - Z
    else if(c >= 65 && c <= 90)
    {
        return 19;
    }
    // s1: (, ), {, }, [, ], ,, ;
    else if(c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';')
    {
        return 20; 
    }
    // s2: =, >, +, *
    else if(c == '=' || c == '>' || c == '+' || c == '*')
    {
        return 21;
    }
    // S3: space, \n, \t
    else if(c == ' ' || c == '\n' || c == '\t')
    {
        return 22;
    }
    else if(c == '"')
    {
        return 23;
    }
    else if(c == "")
    {
        return 24;
    }
    return 25; // invalid column
}

int isFinalState(int state)
{
    if(state == CAT_TYPES || state == CAT_KEYWORD || state == CAT_IDENTIFIER || state == CAT_LITERAL || state == CAT_NONESSENTIAL || state == CAT_NUMBER || state == CAT_SPECIALCHAR || state == CAT_OPERAND)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int FindNextState(char c)
{
    int column = charToColumn(c);
    // to do: matrix of states
    return 0;
}