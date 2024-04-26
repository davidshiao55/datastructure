#include <stdio.h>
#include <stdlib.h>

#define SWAP(x, y, tmp) \
    tmp = x;            \
    x = y;              \
    y = tmp;

typedef struct leftist
{
    struct leftist *leftChild, *rightChild;
    int data;
    int shortest;
} leftist;

void minUnion(leftist **a, leftist **b);
leftist *createTree(int data);
void minMeld(leftist **a, leftist **b);
int pop(leftist **root);
void push(leftist **root, int data);

int main()
{
    leftist *root = createTree(0);
    for (int i = 1; i < 10; i++)
    {
        push(&root, i);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", pop(&root));
    }
    return 0;
}

int pop(leftist **root)
{
    leftist *tmp = *root;
    leftist *a = (*root)->leftChild, *b = (*root)->rightChild;
    int data = (*root)->data;
    minMeld(&a, &b);
    *root = a;
    free(tmp);
    return data;
}

void push(leftist **root, int data)
{
    leftist *t = createTree(data);
    minUnion(root, &t);
}

leftist *createTree(int data)
{
    leftist *t = (leftist *)malloc(sizeof(leftist));
    t->data = data;
    t->leftChild = NULL;
    t->rightChild = NULL;
    t->shortest = 1;
    return t;
}

void minUnion(leftist **a, leftist **b)
{
    leftist *tmp;
    if ((*a)->data > (*b)->data)
    {
        SWAP(*a, *b, tmp);
    }
    if (!(*a)->rightChild)
    {
        (*a)->rightChild = *b;
    }
    else
    {
        minUnion(&(*a)->rightChild, b);
    }

    if (!(*a)->leftChild)
    {
        (*a)->leftChild = (*a)->rightChild;
        (*a)->rightChild = NULL;
    }
    else if ((*a)->leftChild->shortest < (*a)->rightChild->shortest)
    {
        SWAP((*a)->rightChild, (*a)->leftChild, tmp);
    }
    (*a)->shortest = (!(*a)->rightChild) ? 1 : (*a)->rightChild->shortest + 1;
}

void minMeld(leftist **a, leftist **b)
{
    if (!*a)
        *a = *b;
    else if (*b)
        minUnion(a, b);
    *b = NULL;
}