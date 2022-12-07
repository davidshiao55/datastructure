#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    node *llink, *rlink;
} node;

void swapTree(node **root);
void preorder(node **root, int arr[], int n, int *pos);
node *newnode(int data);
void preorderPrint(node *root);
void inorderPrint(node *root);

int main()
{
    int n;
    int pos = 0;
    node *root = NULL;
    scanf("%d", &n);
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    preorder(&root, arr, n, &pos);
    swapTree(&root);
    preorderPrint(root);
    printf("\n");
    inorderPrint(root);
    printf("\n");
    return 0;
}

void swapTree(node **root)
{
    if ((*root)->data == -1)
        return;
    node *tmp = (*root)->llink;
    (*root)->llink = (*root)->rlink;
    (*root)->rlink = tmp;
    swapTree(&(*root)->llink);
    swapTree(&(*root)->rlink);
}

void preorder(node **root, int arr[], int n, int *pos)
{
    if (*pos == n)
        return;
    node *tmp = newnode(arr[(*pos)++]);
    *root = tmp;
    if (tmp->data == -1)
        return;
    preorder(&(*root)->llink, arr, n, pos);
    preorder(&(*root)->rlink, arr, n, pos);
}

void preorderPrint(node *root)
{
    if (!root)
        return;
    if (root->data != -1)
        printf("%d ", root->data);
    else
        return;
    preorderPrint(root->llink);
    preorderPrint(root->rlink);
}

void inorderPrint(node *root)
{
    if (!root)
        return;
    inorderPrint(root->llink);
    if (root->data != -1)
        printf("%d ", root->data);
    else
        return;
    inorderPrint(root->rlink);
}

node *newnode(int data)
{
    node *t = (node *)malloc(sizeof(node));
    t->data = data;
    t->llink = NULL;
    t->rlink = NULL;
    return t;
}
