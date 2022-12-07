#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int data;
    struct node *leftChild, *rightChild;
} node;

int search(int arr[], int strt, int end, int value);
void postOrderPrint(node *root);
void preOrderPrint(node *root);
node *newnode(int data);
node *preinBuildTree(int in[], int pre[], int strt, int end, int *preIndex);
node *posinBuildTree(int in[], int pos[], int strt, int end, int *posIndex);

int main()
{
    int n;
    char com[30];
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%s", com);
        if (!strcmp("preorder-inorder", com))
        {
            int num;
            int preIndex = 0;
            scanf("%d", &num);
            int pre[num], in[num];
            for (int j = 0; j < num; j++)
            {
                scanf("%d", &pre[j]);
            }
            for (int j = 0; j < num; j++)
            {
                scanf("%d", &in[j]);
            }
            node *root = preinBuildTree(in, pre, 0, num - 1, &preIndex);
            postOrderPrint(root);
            printf("\n");
        }
        else if (!strcmp("postorder-inorder", com))
        {
            int num;
            scanf("%d", &num);
            int posIndex = num - 1;
            int pos[num], in[num];
            for (int j = 0; j < num; j++)
            {
                scanf("%d", &pos[j]);
            }
            for (int j = 0; j < num; j++)
            {
                scanf("%d", &in[j]);
            }
            node *root = posinBuildTree(in, pos, 0, num - 1, &posIndex);
            preOrderPrint(root);
            printf("\n");
        }
    }
    return 0;
}

node *posinBuildTree(int in[], int pos[], int strt, int end, int *posIndex)
{
    if (strt > end)
        return NULL;
    node *n = newnode(pos[(*posIndex)--]);
    if (strt == end)
        return n;
    int rootIndex = search(in, strt, end, n->data);
    n->rightChild = posinBuildTree(in, pos, rootIndex + 1, end, posIndex);
    n->leftChild = posinBuildTree(in, pos, strt, rootIndex - 1, posIndex);
    return n;
}

node *preinBuildTree(int in[], int pre[], int strt, int end, int *preIndex)
{
    if (strt > end)
        return NULL;
    node *n = newnode(pre[(*preIndex)++]);
    if (strt == end)
        return n;
    int rootIndex = search(in, strt, end, n->data);
    n->leftChild = preinBuildTree(in, pre, strt, rootIndex - 1, preIndex);
    n->rightChild = preinBuildTree(in, pre, rootIndex + 1, end, preIndex);
    return n;
}

int search(int arr[], int strt, int end, int value)
{
    for (int i = strt; i <= end; i++)
    {
        if (arr[i] == value)
            return i;
    }
}

void preOrderPrint(node *root)
{
    if (root == NULL)
        return;
    printf("%d ", root->data);
    preOrderPrint(root->leftChild);
    preOrderPrint(root->rightChild);
}

void postOrderPrint(node *root)
{
    if (root == NULL)
        return;
    postOrderPrint(root->leftChild);
    postOrderPrint(root->rightChild);
    printf("%d ", root->data);
}

node *newnode(int data)
{
    node *t = (node *)malloc(sizeof(node));
    t->data = data;
    t->rightChild = NULL;
    t->leftChild = NULL;
    return t;
}
