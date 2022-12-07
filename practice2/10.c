#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct threadedTree
{
    bool leftThread, rightThread;
    int id;
    threadedTree *leftChild, *rightChild;
} threadedTree;

threadedTree *insucc(threadedTree *tree);
threadedTree *newnode(int id);
void insertRight(threadedTree *s, threadedTree *r);
void insertLeft(threadedTree *s, threadedTree *r);
void tinorder(threadedTree *root);
threadedTree *tinorderFind(threadedTree *root, int ID);

int main()
{
    int root_id, n;
    char com[20];
    scanf("%d %d", &n, &root_id);
    threadedTree *head = newnode(0);
    threadedTree *root = newnode(root_id);
    head->leftChild = head;
    head->leftThread = true;
    head->rightChild = head;
    insertLeft(head, root);
    for (int i = 0; i < n; i++)
    {
        int parentID, childID;
        scanf("%d %s %d", &parentID, com, &childID);
        threadedTree *node = newnode(childID);
        if (!strcmp(com, "left"))
        {
            insertLeft(tinorderFind(head, parentID), node);
        }
        if (!strcmp(com, "right"))
        {
            insertRight(tinorderFind(head, parentID), node);
        }
    }
    tinorder(head);
    return 0;
}

threadedTree *tinorderFind(threadedTree *root, int ID)
{
    threadedTree *tmp = root;
    root = insucc(root);
    while (root != tmp)
    {
        if (root->id == ID)
            return root;
        root = insucc(root);
    }
    printf("%d doesn't exist\n", ID);
}

void tinorder(threadedTree *root)
{
    threadedTree *tmp = root;
    root = insucc(root);
    while (root != tmp)
    {
        printf("%d ", root->id);
        root = insucc(root);
    }
}

threadedTree *newnode(int id)
{
    threadedTree *n = (threadedTree *)malloc(sizeof(threadedTree));
    n->id = id;
    n->leftChild = NULL;
    n->rightChild = NULL;
    n->leftThread = false;
    n->rightThread = false;
    return n;
}

void insertLeft(threadedTree *s, threadedTree *r)
{
    threadedTree *tmp;
    r->leftChild = s->leftChild;
    r->leftThread = s->leftThread;
    r->rightChild = s;
    r->rightThread = true;
    s->leftChild = r;
    s->leftThread = false;
    if (!r->leftThread)
    {
        tmp = r->leftChild;
        while (!tmp->rightThread)
            tmp = tmp->rightChild;
        tmp->rightChild = r;
    }
}
void insertRight(threadedTree *s, threadedTree *r)
{
    threadedTree *tmp;
    r->rightChild = s->rightChild;
    r->rightThread = s->rightThread;
    r->leftChild = s;
    r->leftThread = true;
    s->rightChild = r;
    s->rightThread = false;
    if (!r->rightThread)
    {
        tmp = insucc(r);
        tmp->leftChild = r;
    }
}

threadedTree *insucc(threadedTree *tree)
{
    threadedTree *tmp;
    tmp = tree->rightChild;
    if (!tree->rightThread)
    {
        while (!tmp->leftThread)
        {
            tmp = tmp->leftChild;
        }
    }
    return tmp;
}