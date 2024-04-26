#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    node *next;
} node;

node *create();
void insert(node **first, node *x);
void deleteNode(node **first, node *trail, node *x);
void printList(node *first);

int main()
{
    node *first = create();
    insert(&first, first);
    insert(&first, first);
    insert(&first, first);
    insert(&first, first);
    insert(&first, first);

    // deleteNode(&first, first->next, first->next->next);
    printList(first);
    return 0;
}

node *create()
{
    node *first = (node *)malloc(sizeof(node));
    node *second = (node *)malloc(sizeof(node));
    first->data = 1;
    first->next = second;
    second->data = 2;
    second->next = NULL;
    return first;
}

// pass by value function裡的 first會指向新的地址但main裡的不會
// 所以要傳 first的地址才會改到main裡的值

void insert(node **first, node *x)
{
    node *tmp;
    tmp = (node *)malloc(sizeof(node));
    tmp->data = 50;
    if (*first)
    {
        tmp->next = x->next;
        x->next = tmp;
    }
    else
    {
        *first = tmp;
        tmp->next = NULL;
    }
}

void deleteNode(node **first, node *trail, node *x)
{
    if (trail)
    {
        trail->next = x->next;
    }
    else
    {
        *first = (*first)->next;
    }
    free(x);
}

void printList(node *first)
{
    for (; first; first = first->next)
    {
        printf("%d\n", first->data);
    }
}