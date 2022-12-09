#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    node *llink;
    char data;
    node *rlink;
} node;

void dinsert(node *n, node *newnode);
void ddelete(node *n, node *deleted);
void printClist(node *p);
node *createNode(char data);

int main()
{
    node *header = (node *)malloc(sizeof(node));
    header->llink = header;
    header->rlink = header;

    dinsert(header, createNode('a'));
    dinsert(header, createNode('b'));
    dinsert(header, createNode('c'));
    ddelete(header, header->rlink);

    printClist(header);
    return 0;
}

void ddelete(node *n, node *deleted)
{
    if (n == deleted)
    {
        printf("Deletion of header node not permitted\n");
    }
    else
    {
        deleted->rlink->llink = deleted->llink;
        deleted->llink->rlink = deleted->rlink;
        free(deleted);
    }
}

node *createNode(char data)
{
    node *n = (node *)malloc(sizeof(node));
    n->data = data;
    return n;
}

void dinsert(node *n, node *newnode)
{
    newnode->llink = n;
    n->rlink->llink = newnode;
    newnode->rlink = n->rlink;
    n->rlink = newnode;
}

void printClist(node *p)
{
    node *startP = p;
    for (p = p->rlink; p != startP; p = p->rlink)
    {
        printf("%c\n", p->data);
    }
}