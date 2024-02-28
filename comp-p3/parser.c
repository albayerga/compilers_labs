// #include "parser.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// char terminals[] = {'+', '*', '(', ')', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
// char non_terminals[] = {'s', 'e', 't', 'f'};

// char rules[][7] =
// {
//     "e",
//     "e+t",
//     "t",
//     "t*f",
//     "f",
//     "(e)",
//     "0",
//     "1",
//     "2",
//     "3",
//     "4",
//     "5",
//     "6",
//     "7",
//     "8",
//     "9"
// };

// bool isTerminal(char c)
// {
//     for (int i = 0; i < sizeof(terminals); i++)
//     {
//         if (c == terminals[i])
//         {
//             return true;
//         }
//     }
//     return false;
// }

// bool isNonTerminal(char c)
// {
//     for (int i = 0; i < sizeof(non_terminals); i++)
//     {
//         if (c == non_terminals[i])
//         {
//             return true;
//         }
//     }
//     return false;
// }

// void initialize(Stack *s)
// {
//     s->top = -1;
// }

// void push(Stack *s, char c)
// {
//     if (s->top < STACK_SIZE - 1)
//     {
//         s->top++;
//         s->items[s->top] = c;
//     }
//     else
//     {
//         fprintf(stderr, "Stack overflow\n");
//         exit(EXIT_FAILURE);
//     }
// }

// char pop(Stack *s)
// {
//     if (s->top >= 0)
//     {
//         char c = s->items[s->top];
//         s->top--;
//         return c;
//     }
//     else
//     {
//         fprintf(stderr, "Stack underflow\n");
//         exit(EXIT_FAILURE);
//     }
// }

// //reduce
// //parse

