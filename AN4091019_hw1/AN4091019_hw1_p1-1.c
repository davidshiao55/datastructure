#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSTACK 1000

char stack[MAXSTACK];
int top = -1;

int priority(char op);
char pop();
void push(char e);
bool isOperator(char e);
bool isEmpty();
bool isFull();

char *inToPos(char input[], int n);

int main()
{
    char input[1000];
    scanf("%s", input);
    char *posfix = inToPos(input, strlen(input));
    printf("%s", posfix);
    return 0;
}

char *inToPos(char input[], int n)
{
    char *result = (char *)malloc(sizeof(char) * n + 1);
    int pos = 0;
    for (int i = 0; i < n; i++)
    {
        if (isOperator(input[i]))
        {
            while (!isEmpty() && priority(input[i]) <= priority(stack[top]))
                result[pos++] = pop();
            push(input[i]);
        }
        else if (input[i] == '(')
        {
            push(input[i]);
        }
        else if (input[i] == ')')
        {
            while (stack[top] != '(')
                result[pos++] = pop();
            pop();
        }
        else
        {
            result[pos++] = input[i];
        }
    }
    while (!isEmpty())
        result[pos++] = pop();
    result[pos] = '\0';
    return result;
}

int priority(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '/':
    case '*':
        return 2;
    case '^':
        return 3;
    default:
        return -1;
    }
}

char pop()
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

void push(char e)
{
    if (isFull())
    {
        perror("stack is full");
        exit(EXIT_FAILURE);
    }
    else
    {
        stack[++top] = e;
    }
}
bool isOperator(char e)
{
    if (e == '+' || e == '-' || e == '*' || e == '/' || e == '^')
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
