#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    node *llink, *rlink;
} node;

void preorder(node **root, int arr[], int n, int *pos);
node *newnode(int data);
void preorderPrint(node *root);
void postorderPrint(node *root);

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
    preorderPrint(root);
    printf("\n");
    postorderPrint(root);
    printf("\n");
    return 0;
}

void postorderPrint(node *root)
{
    if (!root)
        return;
    postorderPrint(root->llink);
    postorderPrint(root->rlink);
    if (root->data != -1)
        printf("%d ", root->data);
    else
        return;
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

// root -> r -> l
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

node *newnode(int data)
{
    node *t = (node *)malloc(sizeof(node));
    t->data = data;
    t->llink = NULL;
    t->rlink = NULL;
    return t;
}
