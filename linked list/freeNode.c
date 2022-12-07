#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    node *link;
} node;

node *getNode();
void retNode(node *tmp);

node *avail = NULL;

int main()
{
    return 0;
}

node *getNode()
{
    node *tmp;
    if (avail)
    {
        tmp = avail;
        avail = avail->link;
    }
    else
    {
        tmp = (node *)malloc(sizeof(node));
    }
    return tmp;
}

void retNode(node *tmp)
{
    tmp->link = avail;
    avail = tmp;
}
