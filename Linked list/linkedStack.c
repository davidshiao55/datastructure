#include <stdio.h>
#include <stdlib.h>

#define MAX_STACKS 10

typedef struct element
{
    int data;
} element;

typedef struct stack
{
    element data;
    stack *link;
} stack;

void printStack();
void push(int i, element item);
element pop(int i);

stack *top[MAX_STACKS];
//  assume top[0 <= i <MAXSTACKS] = NULL
int main()
{
    for (int i = 0; i < MAX_STACKS; i++)
    {
        top[i] = NULL;
    }

    for (int i = 0; i < 6; i++)
    {
        element tmpItem = {i};
        push(i, tmpItem);
    }

    printStack();
    return 0;
}

void push(int i, element item)
{
    stack *tmp = (stack *)malloc(sizeof(stack));
    tmp->data = item;
    tmp->link = top[i];
    top[i] = tmp;
}

element pop(int i)
{
    stack *tmp = top[i];
    element item;
    if (!tmp)
    {
        perror("stack is empty");
        exit(EXIT_FAILURE);
    }
    item = tmp->data;
    top[i] = tmp->link;
    free(tmp);
    return item;
}

void printStack()
{
    for (int i = 0; top[i]; i++)
    {
        printf("%d\n", top[i]->data.data);
    }
}