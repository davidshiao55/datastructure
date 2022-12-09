#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct avlnode
{
    struct avlnode *leftChild;
    struct avlnode *rightChild;
    int key;
    short int bf;
} avlnode;

void preOrder(avlnode *root);
avlnode *newavlnode(int key);
void avlInsert(avlnode **parent, int key, bool *unbalanced);
avlnode *avlSearch(avlnode *root, int key);
void avlDelete(avlnode **parent, int key, bool *unbalanced);
void leftRotation(avlnode **parent, bool *unbalanced);
void rightRotation(avlnode **parent, bool *unbalanced);
void leftRotationOnDelete(avlnode **parent, bool *unbalanced);
void rightRotationOnDelete(avlnode **parent, bool *unbalanced);

int main()
{
    char com[20];
    avlnode *root = NULL;

    while (1)
    {
        scanf("%s", com);

        if (!strcmp(com, "insert"))
        {
            int x;
            bool unbalanced = false;
            scanf("%d", &x);
            avlInsert(&root, x, &unbalanced);
        }

        else if (!strcmp(com, "search"))
        {
            int x;
            avlnode *find;
            scanf("%d", &x);
            if (find = avlSearch(root, x))
                printf("%d\n", find->bf);
            else
                printf("Not found\n");
        }

        else if (!strcmp(com, "delete"))
        {
            int x;
            bool unbalanced = false;
            scanf("%d", &x);
            avlDelete(&root, x, &unbalanced);
        }

        else if (!strcmp(com, "quit"))
        {
            break;
        }

        else if (!strcmp(com, "print"))
        {
            preOrder(root);
            printf("\n");
        }
    }
    return 0;
}

void avlInsert(avlnode **parent, int key, bool *unbalanced)
{
    // insert data into a null tree
    if (!*parent)
    {
        *unbalanced = true;
        *parent = newavlnode(key);
    }
    else if (key < (*parent)->key)
    {
        avlInsert(&(*parent)->leftChild, key, unbalanced);

        // backtrace to balance tree
        if (*unbalanced)
        {
            switch ((*parent)->bf)
            {
            case -1:
                (*parent)->bf = 0;
                *unbalanced = false;
                break;
            case 0:
                (*parent)->bf = 1;
                break;
            case 1:
                leftRotation(parent, unbalanced);
                break;
            }
        }
    }
    else if (key > (*parent)->key)
    {
        avlInsert(&(*parent)->rightChild, key, unbalanced);

        // backtrace to balance tree
        if (*unbalanced)
        {
            switch ((*parent)->bf)
            {
            case 1:
                (*parent)->bf = 0;
                *unbalanced = false;
                break;
            case 0:
                (*parent)->bf = -1;
                break;
            case -1:
                rightRotation(parent, unbalanced);
                break;
            }
        }
    }
    else
    {
        *unbalanced = false;
        // printf("%d key already exist\n", key);
    }
}

avlnode *avlSearch(avlnode *root, int key)
{
    if (!root)
        return NULL;
    if (key == root->key)
        return root;
    else if (key < root->key)
        avlSearch(root->leftChild, key);
    else
        avlSearch(root->rightChild, key);
}

void leftRotation(avlnode **parent, bool *unbalanced)
{
    avlnode *child = (*parent)->leftChild;
    avlnode *grandChild;
    if (child->bf == 1)
    {
        // LL
        (*parent)->leftChild = child->rightChild;
        child->rightChild = *parent;
        (*parent)->bf = 0;
        (*parent) = child;
    }
    else
    {
        // LR
        grandChild = child->rightChild;
        if (!grandChild)
        {
            bool pl = !((*parent)->leftChild == NULL);
            bool r = !(child->rightChild == NULL);
            bool l = !(child->leftChild == NULL);
            fprintf(stderr, "grandchild doesn't exist\n");
            fprintf(stderr, "parent %d bf %d child %d bf %d\n", (*parent)->key, (*parent)->bf, child->key, child->bf);
            fprintf(stderr, "pl %d right %d left %d\n", pl, r, l);
            exit(EXIT_FAILURE);
        }
        child->rightChild = grandChild->leftChild;
        grandChild->leftChild = child;
        (*parent)->leftChild = grandChild->rightChild;
        grandChild->rightChild = *parent;
        switch (grandChild->bf)
        {
        case 1:
            (*parent)->bf = -1;
            child->bf = 0;
            break;
        case 0:
            (*parent)->bf = 0;
            child->bf = 0;
            break;
        case -1:
            (*parent)->bf = 0;
            child->bf = 1;
            break;
        }
        *parent = grandChild;
    }
    (*parent)->bf = 0;
    *unbalanced = false;
}

void rightRotation(avlnode **parent, bool *unbalanced)
{
    avlnode *child = (*parent)->rightChild;
    avlnode *grandChild;
    if (child->bf == -1)
    {
        // RR
        (*parent)->rightChild = child->leftChild;
        child->leftChild = *parent;
        (*parent)->bf = 0;
        (*parent) = child;
    }
    else
    {
        // RL
        grandChild = child->leftChild;
        child->leftChild = grandChild->rightChild;
        grandChild->rightChild = child;
        (*parent)->rightChild = grandChild->leftChild;
        grandChild->leftChild = *parent;
        switch (grandChild->bf)
        {
        case 1:
            (*parent)->bf = 0;
            child->bf = -1;
            break;
        case 0:
            (*parent)->bf = 0;
            child->bf = 0;
            break;
        case -1:
            (*parent)->bf = 1;
            child->bf = 0;
            break;
        }
        *parent = grandChild;
    }
    (*parent)->bf = 0;
    *unbalanced = false;
}

/* every recursive call has to be follow by backtrace code, even when swapping key */
void avlDelete(avlnode **parent, int key, bool *unbalanced)
{
    if (!*parent)
    {
        *unbalanced = false;
        // printf("%d key doesn't exist\n", key);
        return;
    }

    // change the delete node to leaf node
    if (key == (*parent)->key)
    {
        // case 1 : deletenode has two child
        if ((*parent)->leftChild && (*parent)->rightChild)
        {
            avlnode *inorderPredecessor = (*parent)->leftChild;
            while (inorderPredecessor->rightChild)
                inorderPredecessor = inorderPredecessor->rightChild;
            (*parent)->key = inorderPredecessor->key;
            avlDelete(&(*parent)->leftChild, (*parent)->key, unbalanced);

            // backtrace to balance tree
            if (*unbalanced)
            {
                // left branch has grown shorter
                switch ((*parent)->bf)
                {
                // height is decrease by 1, may be unbalanced at upper node
                case 1:
                    (*parent)->bf = 0;
                    break;
                // no change in height
                case 0:
                    (*parent)->bf = -1;
                    *unbalanced = false;
                    break;
                // tree is unbalanced at current node
                case -1:
                    rightRotationOnDelete(parent, unbalanced);
                    break;
                }
            }
        }
        // case 2 : deletenode has one child
        else if ((*parent)->leftChild || (*parent)->rightChild)
        {
            if ((*parent)->leftChild)
            {
                (*parent)->key = (*parent)->leftChild->key;
                avlDelete(&(*parent)->leftChild, (*parent)->key, unbalanced);

                // backtrace to balance tree
                if (*unbalanced)
                {
                    // left branch has grown shorter
                    switch ((*parent)->bf)
                    {
                    // height is decrease by 1, may be unbalanced at upper node
                    case 1:
                        (*parent)->bf = 0;
                        break;
                    // no change in height
                    case 0:
                        (*parent)->bf = -1;
                        *unbalanced = false;
                        break;
                    // tree is unbalanced at current node
                    case -1:
                        rightRotationOnDelete(parent, unbalanced);
                        break;
                    }
                }
            }
            else if ((*parent)->rightChild)
            {
                (*parent)->key = (*parent)->rightChild->key;
                avlDelete(&(*parent)->rightChild, (*parent)->key, unbalanced);

                // backtrace to balance tree
                if (*unbalanced)
                {
                    // right branch has grown shorter
                    switch ((*parent)->bf)
                    {
                    // height is decrease by 1
                    case -1:
                        (*parent)->bf = 0;
                        break;
                    // no change in height
                    case 0:
                        (*parent)->bf = 1;
                        *unbalanced = false;
                        break;
                    // tree is unbalanced at current node
                    case 1:
                        leftRotationOnDelete(parent, unbalanced);
                        break;
                    }
                }
            }
        }
        // case 3 : deletenode is leaf
        else
        {
            free(*parent);
            *parent = NULL;
            *unbalanced = true;
        }
    }
    else if (key < (*parent)->key)
    {
        avlDelete(&(*parent)->leftChild, key, unbalanced);

        // backtrace to balance tree
        if (*unbalanced)
        {
            // left branch has grown shorter
            switch ((*parent)->bf)
            {
            // height is decrease by 1, may be unbalanced at upper node
            case 1:
                (*parent)->bf = 0;
                break;
            // no change in height
            case 0:
                (*parent)->bf = -1;
                *unbalanced = false;
                break;
            // tree is unbalanced at current node
            case -1:
                rightRotationOnDelete(parent, unbalanced);
                break;
            }
        }
    }
    else
    {
        avlDelete(&(*parent)->rightChild, key, unbalanced);

        // backtrace to balance tree
        if (*unbalanced)
        {
            // right branch has grown shorter
            switch ((*parent)->bf)
            {
            // height is decrease by 1
            case -1:
                (*parent)->bf = 0;
                break;
            // no change in height
            case 0:
                (*parent)->bf = 1;
                *unbalanced = false;
                break;
            // tree is unbalanced at current node
            case 1:
                leftRotationOnDelete(parent, unbalanced);
                break;
            }
        }
    }
}

void leftRotationOnDelete(avlnode **parent, bool *unbalanced)
{
    avlnode *child = (*parent)->leftChild;
    avlnode *grandChild;
    if (child->bf == 0)
    {
        // R0
        (*parent)->leftChild = child->rightChild;
        child->rightChild = *parent;
        (*parent)->bf = 1;
        (*parent) = child;
        (*parent)->bf = -1;
        *unbalanced = false;
    }
    else if (child->bf == 1)
    {
        // R1
        (*parent)->leftChild = child->rightChild;
        child->rightChild = *parent;
        (*parent)->bf = 0;
        (*parent) = child;
        (*parent)->bf = 0;
        *unbalanced = true;
    }
    else
    {
        // R-1
        grandChild = child->rightChild;
        child->rightChild = grandChild->leftChild;
        grandChild->leftChild = child;
        (*parent)->leftChild = grandChild->rightChild;
        grandChild->rightChild = *parent;
        switch (grandChild->bf)
        {
        case 1:
            (*parent)->bf = -1;
            child->bf = 0;
            break;
        case 0:
            (*parent)->bf = 0;
            child->bf = 0;
            break;
        case -1:
            (*parent)->bf = 0;
            child->bf = 1;
            break;
        }
        *parent = grandChild;
        (*parent)->bf = 0;
        *unbalanced = true;
    }
}

void rightRotationOnDelete(avlnode **parent, bool *unbalanced)
{
    avlnode *child = (*parent)->rightChild;
    avlnode *grandChild;
    if (child->bf == 0)
    {
        // L0
        (*parent)->rightChild = child->leftChild;
        child->leftChild = *parent;
        (*parent)->bf = -1;
        (*parent) = child;
        (*parent)->bf = 1;
        *unbalanced = false;
    }
    else if (child->bf == -1)
    {
        // L1
        (*parent)->rightChild = child->leftChild;
        child->leftChild = *parent;
        (*parent)->bf = 0;
        (*parent) = child;
        (*parent)->bf = 0;
        *unbalanced = true;
    }
    else
    {
        // RL
        grandChild = child->leftChild;
        child->leftChild = grandChild->rightChild;
        grandChild->rightChild = child;
        (*parent)->rightChild = grandChild->leftChild;
        grandChild->leftChild = *parent;
        switch (grandChild->bf)
        {
        case 1:
            (*parent)->bf = 0;
            child->bf = -1;
            break;
        case 0:
            (*parent)->bf = 0;
            child->bf = 0;
            break;
        case -1:
            (*parent)->bf = 1;
            child->bf = 0;
            break;
        }
        *parent = grandChild;
        (*parent)->bf = 0;
        *unbalanced = true;
    }
}

avlnode *newavlnode(int key)
{
    avlnode *n = (avlnode *)malloc(sizeof(avlnode));
    n->bf = 0;
    n->key = key;
    n->leftChild = NULL;
    n->rightChild = NULL;
    return n;
}

void preOrder(avlnode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->leftChild);
        preOrder(root->rightChild);
    }
}