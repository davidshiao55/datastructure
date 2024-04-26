#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSTACK 1000
#define OPERANDMAXLENGTH 100

char stack[MAXSTACK][OPERANDMAXLENGTH];
int top = -1;

char *pop();
void push(char *e);
bool isOperator(char *e);
bool isEmpty();
bool isFull();

int main()
{
    char input[1000];
    scanf("%s", input);
    for (int i = strlen(input); i >= 0; i--)
    {
        char tmp[OPERANDMAXLENGTH];
        tmp[0] = input[i];
        tmp[1] = '\0';

        if (isOperator(tmp))
        {
            char *op1;
            char *op2;
            op1 = pop();
            op2 = pop();

            char result[OPERANDMAXLENGTH];
            strcpy(result, op1);
            push(strcat(strcat(result, op2), tmp));
        }
        else
        {
            push(tmp);
        }
    }
    printf("%s\n", pop());

    scanf("%s", input);
    for (int i = 0; i < strlen(input); i++)
    {
        char tmp[OPERANDMAXLENGTH];
        tmp[0] = input[i];
        tmp[1] = '\0';

        if (isOperator(tmp))
        {
            char *op1;
            char *op2;
            op1 = pop();
            op2 = pop();

            push(strcat(strcat(tmp, op2), op1));
        }
        else
        {
            push(tmp);
        }
    }
    printf("%s\n", pop());
    return 0;
}

char *pop()
{
    if (isEmpty())
    {
        perror("stack is empty");
        exit(EXIT_FAILURE);
    }
    else
    {
        return stack[top--];
    }
}
void push(char *e)
{
    if (isFull())
    {
        perror("stack is full");
        exit(EXIT_FAILURE);
    }
    else
    {
        strcpy(stack[++top], e);
    }
}
bool isOperator(char *e)
{
    if (e[1] == '\0' && (e[0] == '+' || e[0] == '-' || e[0] == '*' || e[0] == '/' || e[0] == '^'))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isEmpty()
{
    if (top == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isFull()
{
    if (top >= MAXSTACK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
