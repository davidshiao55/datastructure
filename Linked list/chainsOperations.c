#include <stdio.h>
#include <stdlib.h>

typedef struct listNode
{
    char data;
    listNode *link;
} listNode;

listNode *invert(listNode *lead);
listNode *concatenate(listNode *ptr1, listNode *ptr2);
void insertFront(listNode **first, char data);
void printList(listNode *n);

int main()
{
    listNode *first = NULL;
    insertFront(&first, 'a');
    insertFront(&first, 'b');
    insertFront(&first, 'c');
    insertFront(&first, 'd');

    first = invert(first);

    listNode *list2 = NULL;
    insertFront(&list2, 'b');
    insertFront(&list2, 'c');
    insertFront(&list2, 'd');
    insertFront(&list2, 'a');

    first = concatenate(list2, first);
    printList(first);
    return 0;
}

listNode *concatenate(listNode *ptr1, listNode *ptr2)
{
    listNode *tmp;
    if (!ptr1)
        return ptr2;
    if (!ptr2)
        return ptr1;

    for (tmp = ptr1; tmp->link; tmp = tmp->link)
        ;
    tmp->link = ptr2;

    return ptr1;
}

listNode *invert(listNode *lead)
{
    listNode *middle, *trail;
    middle = NULL;
    while (lead)
    {
        trail = middle;
        middle = lead;
        lead = lead->link;
        middle->link = trail;
    }
    return middle;
}

void insertFront(listNode **first, char data)
{
    listNode *n = (listNode *)malloc(sizeof(listNode));
    n->data = data;
    if (!(*first))
    {
        *first = n;
        n->link = NULL;
    }
    else
    {
        n->link = *first;
        *first = n;
    }
}

void printList(listNode *n)
{
    for (; n; n = n->link)
    {
        printf("%c\n", n->data);
    }
}