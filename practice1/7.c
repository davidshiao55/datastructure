#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

typedef struct node
{
    bool flag;
    node *down;
    char data;
    node *next;
} node;

typedef struct tree
{
    int treeID;
    node *root;
} tree;

void read(node **root, char input[]);
void write(node *root);
node *newnode(bool flag, char data);
void copy(node **dest, node *source);
bool isEqual(node *a, node *b);
void clear(node **root);

int pos = 0;

int main()
{
    int n;
    char com[20];
    tree treeList[10];
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%s", com);
        if (!strcmp(com, "read"))
        {
            pos = 0;
            int treeID;
            char input[MAX_STRING_LENGTH];
            scanf("%d %s", &treeID, input);
            node *head = newnode(false, input[1]);
            input[strlen(input) - 1] = '\0';
            read(&head, &input[3]);
            treeList[treeID].root = head;
            treeList[treeID].treeID = treeID;
            printf("readed\n");
        }
        if (!strcmp(com, "write"))
        {
            int treeID;
            scanf("%d", &treeID);
            printf("(");
            write(treeList[treeID].root);
            printf(")\n");
        }
        if (!strcmp(com, "copy"))
        {
            int srcID, destID;
            node *head = NULL;
            scanf("%d %d", &srcID, &destID);
            treeList[destID].root = head;
            treeList[destID].treeID = destID;
            copy(&treeList[destID].root, treeList[srcID].root);
            printf("copied\n");
        }
        if (!strcmp(com, "isequal"))
        {
            int tree1, tree2;
            scanf("%d %d", &tree1, &tree2);
            printf(isEqual(treeList[tree1].root, treeList[tree2].root) ? "true\n" : "false\n");
        }
        if (!strcmp(com, "clear"))
        {
            int treeID;
            scanf("%d", &treeID);
            clear(&treeList[treeID].root);
            printf("cleared\n");
        }
    }
    return 0;
}

void clear(node **root)
{
    if ((*root)->flag)
    {
        clear(&(*root)->down);
        clear(&(*root)->next);
        free(*root);
    }
    else
    {
        if ((*root)->next)
            clear(&(*root)->next);
        free(*root);
    }
}

bool isEqual(node *a, node *b)
{
    if (!a->flag && !b->flag)
    {
        if (a->data != b->data)
            return false;
        if (a->next && b->next && !isEqual(a->next, b->next))
            return false;
    }
    else if (a->flag && b->flag)
    {
        if (!isEqual(a->down, b->down))
            return false;
        if (!isEqual(a->next, b->next))
            return false;
    }
    else
    {
        return false;
    }
    return true;
}

void copy(node **dest, node *source)
{
    if (!source->flag)
    {
        *dest = newnode(source->flag, source->data);
        if (source->next)
        {
            copy(&(*dest)->next, source->next);
        }
    }
    else
    {
        *dest = newnode(source->flag, source->data);
        copy(&(*dest)->down, source->down);
        copy(&(*dest)->next, source->next);
    }
}

void write(node *root)
{
    if (!root->flag)
    {
        printf("%c", root->data);
        if (root->next)
        {
            printf("(");
            write(root->next);
            printf(")");
        }
    }
    else
    {
        write(root->down);
        printf(",");
        write(root->next);
    }
}

void read(node **root, char input[])
{
    while (input[pos] != '\0')
    {
        switch (input[pos])
        {
        case '(':
        {
            pos++;
            node *nextRoot = (*root)->next;
            while (nextRoot->flag)
                nextRoot = nextRoot->next;
            read(&nextRoot, input);
            break;
        }
        case ',':
        {
            node *prev = *root;
            while (prev->next->flag)
                prev = prev->next;
            pos++;
            node *n = newnode(true, 0);
            n->down = prev->next;
            prev->next = n;
            break;
        }
        case ')':
        {
            pos++;
            // sort node
            node *d, *d2 = (*root)->next;
            node *next, *tmp;
            while (d2->flag)
            {
                d = (*root)->next;
                while (d->flag)
                {
                    next = d->next;
                    if (next->flag)
                        next = next->down;
                    if (d->down->data > next->data)
                    {
                        tmp = d->down;
                        d->down = next;
                        d->next = tmp;
                    }
                    d = d->next;
                }
                d2 = d2->next;
            }
            return;
        }
        default:
        {
            node *n = newnode(false, input[pos++]);
            node *loc = *root;
            while (loc->next)
                loc = loc->next;
            loc->next = n;
            break;
        }
        }
        // pos++;
    }
}

node *newnode(bool flag, char data)
{
    node *n = (node *)malloc(sizeof(node));
    n->flag = flag;
    n->data = data;
    n->down = NULL;
    n->next = NULL;
    return n;
}
