#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char terminals[] = {'+', '*', '(', ')', 'NUM'};
char non_terminals[] = {'s', 'e', 't', 'f'};

char rules[][4] =
{
    "e",
    "e+t",
    "t",
    "t*f",
    "f",
    "(e)",
    "NUM"
};

void initialize(Stack *s)
{
    s->top = -1;
}

void push(Stack *s, char c)
{
    if (s->top < STACK_SIZE - 1)
    {
        s->top++;
        s->items[s->top] = c;
    }
    else
    {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

char pop(Stack *s)
{
    if (s->top >= 0)
    {
        char c = s->items[s->top];
        s->top--;
        return c;
    }
    else
    {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

bool isTerminal(char c)
{
    for (int i = 0; i < sizeof(terminals); i++)
    {
        if (c == terminals[i])
        {
            return true;
        }
    }
    return false;
}

bool isNonTerminal(char c)
{
    for (int i = 0; i < sizeof(non_terminals); i++)
    {
        if (c == non_terminals[i])
        {
            return true;
        }
    }
    return false;
}

char *reduce(Stack *s)
{
    for (int i = sizeof(rules) / sizeof(rules[0]) - 1; i >= 0; i--)
    {
        if (strstr(s->items, rules[i]) == s->items)
        {
            for (int j = 0; j < strlen(rules[i]); j++)
            {
                pop(s);
            }
            return &rules[i][0];
        }
    }
    return NULL;
}

bool parse(char *input_string)
{
    Stack stack;
    initialize(&stack);
    push(&stack, 's');

    char *token = strtok(input_string, " ");
    while (token != NULL)
    {
        char top_of_stack = stack.items[stack.top];

        if (isTerminal(top_of_stack))
        {
            if (top_of_stack == token[0])
            {
                pop(&stack);
                token = strtok(NULL, " ");
            }
            else
            {
                fprintf(stderr, "Error: Terminal mismatch\n");
                return false;
            }
        }
        else if (isNonTerminal(top_of_stack))
        {
            char *reduced = reduce(&stack);
            if (reduced != NULL)
            {
                push(&stack, reduced[0]);
            }
            else
            {
                fprintf(stderr, "Error: Invalid input\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid symbol on stack\n");
            return false;
        }
    }

    if (stack.top == 0 && stack.items[stack.top] == 's' && token == NULL)
    {
        return true;
    }
    else
    {
        fprintf(stderr, "Error: Invalid input.\n");
        return false;
    }
}
