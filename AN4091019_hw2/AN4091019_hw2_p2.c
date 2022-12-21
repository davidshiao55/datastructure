#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CAPACITY 50000

typedef struct node
{
    struct node *parent;
    struct node *child;
    struct node *left;
    struct node *right;
    int key;
    int val;
    int degree;
    bool childCutFlag;
} node;

typedef struct hashItem
{
    int key;
    int value;
    node *nptr;
    struct hashItem *next;
} hashItem;

typedef struct hashTable
{
    hashItem **items;
    int size;
    int count;
} hashTable;

// utiliy function to
// create node
node *newnode(int key, int val);
// insert & delete from circular linked list
void circularInsert(node *n1, node *n2);
void circularDelete(node *n);

// F heap function
void insert(node *n);
int extract();
void delete(node *deletenode);
void decrease(node *n, int y);
void cascadingCut(node *curr);

// hash table function
int hash(int key);
hashTable *ht_create();
hashItem *newItem(int key, int value, node *nptr);
void ht_destroy(hashTable *table);
void ht_insert(hashTable *table, int key, int value, node *nptr);
void ht_delete(hashTable *table, int key, int value);
node *ht_search(hashTable *table, int key, int val);

node *min = NULL;

int main()
{
    node **tmp = &min;
    hashTable *table = ht_create();
    char cmd[20];
    while (1)
    {
        scanf("%s", cmd);
        if (!strcmp(cmd, "insert"))
        {
            int key, val;
            scanf("%d %d", &key, &val);
            node *n = newnode(key, val);
            insert(n);
            ht_insert(table, key, val, n);
        }

        else if (!strcmp(cmd, "extract"))
        {
            if (min)
            {
                int minKey = min->key;
                int minVal = extract();
                printf("(%d)%d\n", minKey, minVal);
                ht_delete(table, minKey, minVal);
            }
        }

        else if (!strcmp(cmd, "delete"))
        {
            int key, val;
            scanf("%d %d", &key, &val);
            node *n = ht_search(table, key, val);

            if (n)
            {
                delete (n);
                ht_delete(table, key, val);
            }
        }

        else if (!strcmp(cmd, "decrease"))
        {
            int x, val, y;
            scanf("%d %d %d", &x, &val, &y);
            node *n = ht_search(table, x, val);

            if (n)
            {
                ht_delete(table, n->key, n->val);
                decrease(n, y);
                ht_insert(table, n->key, n->val, n);
            }
        }

        else if (!strcmp(cmd, "quit"))
        {
            break;
        }
    }
    ht_destroy(table);
    return 0;
}

void insert(node *n)
{
    if (!min)
    {
        min = n;
    }
    else
    {
        circularInsert(min, n);
        if (min->key > n->key)
            min = n;
    }
}

int extract()
{
    /* remove minimum phase */

    // child will always be > parent, min will not change when inserting child
    int minVal = min->val;

    node *child;
    for (int i = 0; i < min->degree; i++)
    {
        child = min->child;
        circularDelete(child);
        circularInsert(min, child);
    }

    // min is the only node left
    if (min == min->right)
    {
        free(min);
        min = NULL;
        return minVal;
    }

    int maxdegree = 0;
    int numRootList = 0;
    node *nextMin = min->right;
    node *tmp = min->right;
    while (tmp != min)
    {
        if (tmp->key < nextMin->key)
            nextMin = tmp;

        if (tmp->degree > maxdegree)
            maxdegree = tmp->degree;

        numRootList++;
        tmp = tmp->right;
    }
    circularDelete(min);
    free(min);
    min = nextMin;

    /* clean up phase */

    // maxdegree = max degree in root list + log2(numRootList) + 1 (include 0 degree) < ... + numRootList / 2 + 1
    maxdegree += numRootList / 2 + 1;

    node *degreeArray[maxdegree];
    node *rootList[numRootList];

    for (int i = 0; i < maxdegree; i++)
        degreeArray[i] = NULL;

    tmp = min;
    for (int i = 0; i < numRootList; i++, tmp = tmp->right)
        rootList[i] = tmp;

    for (int i = 0; i < numRootList; i++)
    {
        node *curr = rootList[i];
        // isolate node in rootlist so that circular link list is functional after insert
        circularDelete(curr);

        while (degreeArray[curr->degree])
        {
            curr->childCutFlag = false;
            degreeArray[curr->degree]->childCutFlag = false;

            if (curr->key < degreeArray[curr->degree]->key)
            {
                if (curr->child)
                    circularInsert(curr->child, degreeArray[curr->degree]);
                else
                    curr->child = degreeArray[curr->degree];
                degreeArray[curr->degree]->parent = curr;
                curr = curr;
                degreeArray[curr->degree] = NULL;
                curr->degree++;
            }
            else
            {
                if (degreeArray[curr->degree]->child)
                    circularInsert(degreeArray[curr->degree]->child, curr);
                else
                    degreeArray[curr->degree]->child = curr;
                curr->parent = degreeArray[curr->degree];
                curr = degreeArray[curr->degree];
                degreeArray[curr->degree] = NULL;
                curr->degree++;
            }
        }
        degreeArray[curr->degree] = curr;
    }

    /* rebuild tree phase */

    for (int i = 0; i < maxdegree; i++)
    {
        if (degreeArray[i] && degreeArray[i] != min)
            circularInsert(min, degreeArray[i]);
    }
    return minVal;
}

void delete(node *deletenode)
{
    if (!min)
    {
        printf("heap is empty\n");
        return;
    }

    node *child;
    for (int i = 0; i < deletenode->degree; i++)
    {
        child = deletenode->child;
        circularDelete(child);
        circularInsert(min, child);
    }

    /* case 1 : no parent*/
    if (!deletenode->parent)
    {
        if (deletenode == min)
            extract();
        else
            circularDelete(deletenode);
    }
    /* case 2 : parent is not marked*/
    else if (!deletenode->parent->childCutFlag)
    {
        deletenode->parent->childCutFlag = true;
        deletenode->parent->degree--;
        circularDelete(deletenode);
        insert(deletenode);
    }
    /* case 3 : parent is marked*/
    else if (deletenode->parent->childCutFlag)
    {
        node *parent = deletenode->parent;
        deletenode->parent->degree--;
        circularDelete(deletenode);
        insert(deletenode);
        cascadingCut(parent);
    }
}

void decrease(node *n, int y)
{
    if (!min)
    {
        printf("heap is empty\n");
        return;
    }
    n->key -= y;
    /* case 1 : heap property not violated */
    if (!n->parent || n->key > n->parent->key)
    {
        if (n->key < min->key)
            min = n;
        return;
    }

    /* case 2 : heap property violated & parent is not marked*/
    if (!n->parent->childCutFlag)
    {
        n->parent->childCutFlag = true;
        n->parent->degree--;
        circularDelete(n);
        insert(n);
    }
    /* case 3 : heap property violated & parent is marked*/
    else if (n->parent->childCutFlag)
    {
        node *parent = n->parent;
        n->parent->degree--;
        circularDelete(n);
        insert(n);
        cascadingCut(parent);
    }
}

void cascadingCut(node *curr)
{
    node *parent = curr->parent;
    while (curr->childCutFlag)
    {
        if (parent)
        {
            parent->degree--;
            parent = curr->parent;
        }
        else
        {
            /* stop when reach root List*/
            break;
        }
        circularDelete(curr);
        insert(curr);
        curr = parent;
    }
    curr->childCutFlag = true;
}

void circularInsert(node *n1, node *n2)
{
    if (n1 == n2)
        return;
    n1->left->right = n2;
    n2->left = n1->left;
    n1->left = n2;
    n2->right = n1;
}

void circularDelete(node *n)
{
    // handle child pointer after deletion
    if (n->parent && n->parent->child == n)
    {
        // n is the only child
        if (n->right == n)
            n->parent->child = NULL;
        else
            n->parent->child = n->right;
    }
    n->left->right = n->right;
    n->right->left = n->left;
    n->left = n;
    n->right = n;
    n->parent = NULL;
}

node *newnode(int key, int val)
{
    node *n = (node *)malloc(sizeof(node));
    if (!n)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    n->child = NULL;
    n->parent = NULL;
    n->right = n;
    n->left = n;
    n->childCutFlag = false;
    n->degree = 0;
    n->key = key;
    n->val = val;
    return n;
}

hashTable *ht_create()
{
    hashTable *table = (hashTable *)malloc(sizeof(hashTable));
    if (!table)
    {
        fprintf(stderr, "table create error\n");
        exit(EXIT_FAILURE);
    }
    table->size = CAPACITY;
    table->count = 0;

    table->items = (hashItem **)malloc(sizeof(hashItem *) * table->size);
    if (!table->items)
    {
        fprintf(stderr, "table create error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < table->size; i++)
    {
        table->items[i] = NULL;
    }

    return table;
}

void ht_destroy(hashTable *table)
{
    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i])
        {
            hashItem *prev = table->items[i];
            hashItem *ti = table->items[i]->next;
            while (ti)
            {
                free(prev);
                prev = ti;
                ti = ti->next;
            }
            free(prev);
        }
    }
    free(table->items);
    free(table);
}

hashItem *newItem(int key, int value, node *nptr)
{
    hashItem *i = (hashItem *)malloc(sizeof(hashItem));
    i->key = key;
    i->value = value;
    i->nptr = nptr;
    i->next = NULL;
    return i;
}

void ht_insert(hashTable *table, int key, int value, node *nptr)
{
    int index = hash(key);

    hashItem *i = newItem(key, value, nptr);
    if (table->items[index])
    {
        hashItem *ti = table->items[index];
        while (ti->next)
            ti = ti->next;
        ti->next = i;
    }
    else
    {
        table->items[index] = i;
    }
    table->count++;
}

node *ht_search(hashTable *table, int key, int val)
{
    int index = hash(key);
    hashItem *ti = table->items[index];
    while (ti)
    {
        if (ti->value == val)
        {
            break;
        }
        ti = ti->next;
    }
    if (!ti)
    {
        // printf("search error : key value pair doesn't exist\n");
        return NULL;
    }
    return ti->nptr;
}

void ht_delete(hashTable *table, int key, int value)
{
    int index = hash(key);
    hashItem *ti = table->items[index];
    hashItem *prev = NULL;
    while (ti)
    {
        if (ti->value == value)
        {
            break;
        }
        prev = ti;
        ti = ti->next;
    }
    if (!ti)
    {
        // printf("delete error : key value pair doesn't exist\n");
        return;
    }

    if (!prev)
        table->items[index] = ti->next;
    else
        prev->next = ti->next;
    free(ti);
}

int hash(int key)
{
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % CAPACITY;
}