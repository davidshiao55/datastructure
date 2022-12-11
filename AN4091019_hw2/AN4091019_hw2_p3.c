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

typedef struct rbnode
{
    struct rbnode *leftChild;
    struct rbnode *rightChild;
    struct rbnode *parent;
    int key;
    char color;
} rbnode;

avlnode *avlSearch(avlnode *root, int key);
void avlInsert(avlnode **parent, int key, bool *unbalanced);
void avlDelete(avlnode **parent, int key, bool *unbalanced);
void leftRotationOnInsert(avlnode **parent, bool *unbalanced);
void rightRotationOnInsert(avlnode **parent, bool *unbalanced);
void leftRotationOnDelete(avlnode **parent, bool *unbalanced);
void rightRotationOnDelete(avlnode **parent, bool *unbalanced);
avlnode *newavlnode(int key);

rbnode *rbSearch(rbnode *root, int key);
void rbInsert(rbnode **root, int key, int *redCount);
void rbDelete(rbnode **root, int key, bool *doubleBlack);
void newrbnode(rbnode *externalNode, int key);
void rbDoubleRedL(rbnode **parent, int *redCount);
void rbDoubleRedR(rbnode **parent, int *redCount);
void rbDoubleBlackL(rbnode **parent, bool *doubleBlack);
void rbDoubleBlackR(rbnode **parent, bool *doubleBlack);

// utility debug function to check rbtree is valid
int computeBlackHeight(rbnode *currNode);
bool consecRed(rbnode *currNode, int red);
bool isRBTreeValid(rbnode *root);

int main()
{
    char mode[20];

    scanf("%s", mode);
    if (!strcmp(mode, "AVL"))
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
        }
    }
    else if (!strcmp(mode, "red_black"))
    {
        char com[20];
        rbnode *root = NULL;
        while (1)
        {
            scanf("%s", com);
            if (!strcmp(com, "insert"))
            {
                int x;
                int redCount = 0;
                scanf("%d\n", &x);
                rbInsert(&root, x, &redCount);
                // color the root back to black
                root->color = 'B';
            }

            else if (!strcmp(com, "search"))
            {
                int x;
                rbnode *find;
                scanf("%d", &x);
                if (find = rbSearch(root, x))
                    printf(find->color == 'R' ? "red\n" : "black\n");
                else
                    printf("Not found\n");
            }

            else if (!strcmp(com, "delete"))
            {
                int x;
                bool doubleBlack = false;
                scanf("%d", &x);
                rbDelete(&root, x, &doubleBlack);
            }

            if (!strcmp(com, "quit"))
            {
                break;
            }
        }
    }
    return 0;
}

rbnode *rbSearch(rbnode *root, int key)
{
    if (!root)
        return NULL;
    if (key == root->key)
        return root;
    else if (key < root->key)
        rbSearch(root->leftChild, key);
    else
        rbSearch(root->rightChild, key);
}

void rbInsert(rbnode **root, int key, int *redCount)
{
    // tree is empty
    if (!*root)
    {
        *root = (rbnode *)malloc(sizeof(rbnode));
        if (!*root)
        {
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE);
        }
        newrbnode(*root, key);
        (*root)->color = 'B';
        (*root)->parent = NULL;
    }
    // external node
    else if ((*root)->key == -1)
    {
        newrbnode(*root, key);
        (*redCount)++;
    }
    else if (key < (*root)->key)
    {
        rbInsert(&(*root)->leftChild, key, redCount);

        switch (*redCount)
        {
        case 1:
            if ((*root)->color == 'R')
                (*redCount)++;
            else
                (*redCount) = 0;
            break;
        case 2:
            rbDoubleRedL(root, redCount);
            break;
        }
    }
    else if (key > (*root)->key)
    {
        rbInsert(&(*root)->rightChild, key, redCount);
        switch (*redCount)
        {
        case 1:
            if ((*root)->color == 'R')
                (*redCount)++;
            else
                (*redCount) = 0;
            break;
        case 2:
            rbDoubleRedR(root, redCount);
            break;
        }
    }
    else
    {
        // printf("key already exist\n");
        return;
    }
}

void rbDelete(rbnode **parent, int key, bool *doubleBlack)
{
    if ((*parent)->key == -1)
    {
        // printf("%d key doesn't exist\n", key);
        return;
    }

    // change the delete node to leaf node
    if (key == (*parent)->key)
    {
        // case 1 : deletenode has two child
        if ((*parent)->leftChild->key != -1 && (*parent)->rightChild->key != -1)
        {
            rbnode *inorderPredecessor = (*parent)->leftChild;
            while (inorderPredecessor->rightChild->key != -1)
                inorderPredecessor = inorderPredecessor->rightChild;
            (*parent)->key = inorderPredecessor->key;
            rbDelete(&(*parent)->leftChild, (*parent)->key, doubleBlack);

            // backtrace to balance tree
            if (*doubleBlack)
                rbDoubleBlackR(parent, doubleBlack);
        }
        // case 2 : deletenode has one child
        else if ((*parent)->leftChild->key != -1 || (*parent)->rightChild->key != -1)
        {
            if ((*parent)->leftChild->key != -1)
            {
                (*parent)->key = (*parent)->leftChild->key;
                rbDelete(&(*parent)->leftChild, (*parent)->key, doubleBlack);

                // backtrace to balance tree
                if (*doubleBlack)
                    rbDoubleBlackR(parent, doubleBlack);
            }
            else if ((*parent)->rightChild->key != -1)
            {
                (*parent)->key = (*parent)->rightChild->key;
                rbDelete(&(*parent)->rightChild, (*parent)->key, doubleBlack);

                // backtrace to balance tree
                if (*doubleBlack)
                    rbDoubleBlackL(parent, doubleBlack);
            }
        }
        // case 3 : deletenode is leaf
        else if ((*parent)->leftChild->key == -1 && (*parent)->rightChild->key == -1)
        {
            free((*parent)->leftChild);
            free((*parent)->rightChild);
            if (!(*parent)->parent)
            {
                free(*parent);
                *parent = NULL;
            }
            else
            {
                if ((*parent)->color == 'B')
                {
                    *doubleBlack = true;
                }
                (*parent)->key = -1;
                (*parent)->color = 'B';
                (*parent)->leftChild = NULL;
                (*parent)->rightChild = NULL;
            }
        }
    }
    else if (key < (*parent)->key)
    {
        rbDelete(&(*parent)->leftChild, key, doubleBlack);

        // backtrace to balance tree
        if (*doubleBlack)
            rbDoubleBlackR(parent, doubleBlack);
    }
    else
    {
        rbDelete(&(*parent)->rightChild, key, doubleBlack);

        // backtrace to balance tree
        if (*doubleBlack)
            rbDoubleBlackL(parent, doubleBlack);
    }
}

void rbDoubleBlackL(rbnode **parent, bool *doubleBlack)
{
    // child is the sibling of the delete node
    rbnode *child = (*parent)->leftChild;
    rbnode *grandChild;
    if (child->color == 'B')
    {
        // y2 case 1
        if (child->leftChild->color == 'R')
        {
            // LL
            grandChild = child->leftChild;
            child->color = (*parent)->color;
            (*parent)->color = 'B';
            grandChild->color = 'B';

            child->parent = (*parent)->parent;
            (*parent)->parent = child;

            (*parent)->leftChild = child->rightChild;
            child->rightChild = *parent;
            (*parent) = child;
            *doubleBlack = false;
        }
        else if (child->rightChild->color == 'R')
        {
            // LR
            grandChild = child->rightChild;
            grandChild->color = (*parent)->color;
            (*parent)->color = 'B';
            child->color = 'B';

            grandChild->parent = (*parent)->parent;
            child->parent = grandChild;
            (*parent)->parent = grandChild;

            child->rightChild = grandChild->leftChild;
            grandChild->leftChild = child;
            (*parent)->leftChild = grandChild->rightChild;
            grandChild->rightChild = *parent;
            *parent = grandChild;
            *doubleBlack = false;
        }
        // y2 case 2
        else
        {
            if ((*parent)->color == 'R')
                *doubleBlack = false;
            else if (!(*parent)->parent)
                *doubleBlack = false;
            else
                *doubleBlack = true;

            child->color = 'R';
            (*parent)->color = 'B';
        }
    }
    // y2 case 3
    else if (child->color == 'R')
    {
        // LL
        child->color = 'B';
        (*parent)->color = 'R';

        child->parent = (*parent)->parent;
        (*parent)->parent = child;

        (*parent)->leftChild = child->rightChild;
        child->rightChild = *parent;
        (*parent) = child;
        *doubleBlack = true;
        rbDoubleBlackL(&(*parent)->rightChild, doubleBlack);
    }
}

void rbDoubleBlackR(rbnode **parent, bool *doubleBlack)
{
    // child is the sibling of the delete node
    rbnode *child = (*parent)->rightChild;
    rbnode *grandChild;
    if (child->color == 'B')
    {
        // y2 case 1
        if (child->rightChild->color == 'R')
        {
            // RR
            grandChild = child->rightChild;
            child->color = (*parent)->color;
            (*parent)->color = 'B';
            grandChild->color = 'B';

            child->parent = (*parent)->parent;
            (*parent)->parent = child;

            (*parent)->rightChild = child->leftChild;
            child->leftChild = *parent;
            (*parent) = child;
            *doubleBlack = false;
        }
        else if (child->leftChild->color == 'R')
        {
            // RL
            grandChild = child->leftChild;
            grandChild->color = (*parent)->color;
            (*parent)->color = 'B';
            child->color = 'B';

            grandChild->parent = (*parent)->parent;
            child->parent = grandChild;
            (*parent)->parent = grandChild;

            child->leftChild = grandChild->rightChild;
            grandChild->rightChild = child;
            (*parent)->rightChild = grandChild->leftChild;
            grandChild->leftChild = *parent;
            *parent = grandChild;
            *doubleBlack = false;
        }
        // y2 case 2
        else
        {
            if ((*parent)->color == 'R')
                *doubleBlack = false;
            else if (!(*parent)->parent)
                *doubleBlack = false;
            else
                *doubleBlack = true;
            child->color = 'R';
            (*parent)->color = 'B';
        }
    }
    // y2 case 3
    else if (child->color == 'R')
    {
        // RR
        child->color = 'B';
        (*parent)->color = 'R';

        child->parent = (*parent)->parent;
        (*parent)->parent = child;

        (*parent)->rightChild = child->leftChild;
        child->leftChild = *parent;
        (*parent) = child;
        *doubleBlack = true;
        rbDoubleBlackR(&(*parent)->leftChild, doubleBlack);
    }
}

void rbDoubleRedL(rbnode **parent, int *redCount)
{
    rbnode *child = (*parent)->leftChild;
    rbnode *grandChild;
    if ((*parent)->rightChild->color == 'R')
    {
        // recolor
        (*parent)->color = 'R';
        child->color = 'B';
        (*parent)->rightChild->color = 'B';
        *redCount = 1;
    }
    else if (child->leftChild->color == 'R')
    {
        // LL
        grandChild = child->leftChild;
        (*parent)->color = 'R';
        child->color = 'B';
        grandChild->color = 'R';

        child->parent = (*parent)->parent;
        (*parent)->parent = child;

        (*parent)->leftChild = child->rightChild;
        child->rightChild = *parent;
        (*parent) = child;
        *redCount = 0;
    }
    else
    {
        // LR
        grandChild = child->rightChild;
        (*parent)->color = 'R';
        child->color = 'R';
        grandChild->color = 'B';

        grandChild->parent = (*parent)->parent;
        child->parent = grandChild;
        (*parent)->parent = grandChild;

        child->rightChild = grandChild->leftChild;
        grandChild->leftChild = child;
        (*parent)->leftChild = grandChild->rightChild;
        grandChild->rightChild = *parent;
        *parent = grandChild;
        *redCount = 0;
    }
}

void rbDoubleRedR(rbnode **parent, int *redCount)
{
    rbnode *child = (*parent)->rightChild;
    rbnode *grandChild;
    if ((*parent)->leftChild->color == 'R')
    {
        // recolor
        (*parent)->color = 'R';
        child->color = 'B';
        (*parent)->leftChild->color = 'B';
        *redCount = 1;
    }
    else if (child->rightChild->color == 'R')
    {
        // RR
        grandChild = child->rightChild;
        (*parent)->color = 'R';
        child->color = 'B';
        grandChild->color = 'R';

        child->parent = (*parent)->parent;
        (*parent)->parent = child;

        (*parent)->rightChild = child->leftChild;
        child->leftChild = *parent;
        (*parent) = child;
        *redCount = 0;
    }
    else
    {
        // RL
        grandChild = child->leftChild;
        (*parent)->color = 'R';
        child->color = 'R';
        grandChild->color = 'B';

        grandChild->parent = (*parent)->parent;
        child->parent = grandChild;
        (*parent)->parent = grandChild;

        child->leftChild = grandChild->rightChild;
        grandChild->rightChild = child;
        (*parent)->rightChild = grandChild->leftChild;
        grandChild->leftChild = *parent;
        *parent = grandChild;
        *redCount = 0;
    }
}

void newrbnode(rbnode *externalNode, int key)
{
    externalNode->key = key;
    externalNode->color = 'R';

    externalNode->leftChild = (rbnode *)malloc(sizeof(rbnode));
    externalNode->rightChild = (rbnode *)malloc(sizeof(rbnode));
    if (!externalNode->leftChild || !externalNode->rightChild)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    externalNode->leftChild->color = 'B';
    externalNode->leftChild->key = -1;
    externalNode->leftChild->leftChild = NULL;
    externalNode->leftChild->rightChild = NULL;
    externalNode->leftChild->parent = externalNode;

    externalNode->rightChild->color = 'B';
    externalNode->rightChild->key = -1;
    externalNode->rightChild->leftChild = NULL;
    externalNode->rightChild->rightChild = NULL;
    externalNode->rightChild->parent = externalNode;
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
                leftRotationOnInsert(parent, unbalanced);
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
                rightRotationOnInsert(parent, unbalanced);
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

void leftRotationOnInsert(avlnode **parent, bool *unbalanced)
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

void rightRotationOnInsert(avlnode **parent, bool *unbalanced)
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
        // L-1
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
    if (!n)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    n->bf = 0;
    n->key = key;
    n->leftChild = NULL;
    n->rightChild = NULL;
    return n;
}

int computeBlackHeight(rbnode *currNode)
{
    // For an empty subtree the answer is obvious
    if (currNode == NULL)
        return 0;
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackHeight(currNode->leftChild);
    int rightHeight = computeBlackHeight(currNode->rightChild);
    int add = currNode->color == 'B' ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1;
    else
        return leftHeight + add;
}

bool consecRed(rbnode *currNode, int red)
{
    if (!currNode)
        return true;
    if (red == 2)
        return false;
    if (currNode->color == 'R')
        (red)++;
    else
        red = 0;
    return consecRed(currNode->leftChild, red) && consecRed(currNode->rightChild, red);
}

bool isRBTreeValid(rbnode *root)
{
    bool blackHeightValid = (computeBlackHeight(root) != -1);
    bool consecRedValid = consecRed(root, 0);
    if (!blackHeightValid)
        printf("black height violation\n");
    if (!consecRedValid)
        printf("consecRed violation\n");
    return blackHeightValid && consecRedValid;
}