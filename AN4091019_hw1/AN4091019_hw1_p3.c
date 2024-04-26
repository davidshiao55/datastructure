#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct edge
{
    int src;
    int dest;
    int weight;
} edge;

void push(edge item, int *n, int e, edge heap[]);
edge pop(int *n, edge heap[]);

void weightUnion(int a, int b, int sets[]);
int collapsingFind(int i, int sets[]);

int main()
{
    int v, e;
    double cost = 0;
    scanf("%d %d", &v, &e);
    edge heap[e];
    int n = -1;

    for (int i = 0; i < e; i++)
    {
        int start, end, weight;
        scanf("%d %d %d", &start, &end, &weight);
        edge newEdge = {start, end, weight};
        push(newEdge, &n, e, heap);
    }

    int sets[v];
    for (int i = 0; i < v; i++)
        sets[i] = -1;

    while (n != -1)
    {
        edge e = pop(&n, heap);
        if (collapsingFind(e.dest, sets) == collapsingFind(e.src, sets))
            continue;
        weightUnion(e.src, e.dest, sets);
        cost += (double)e.weight;
    }
    printf("%.0lf\n", cost);
    return 0;
}

void push(edge item, int *n, int e, edge heap[])
{
    int i;
    if ((*n) >= e)
    {
        printf("full. \n");
        return;
    }
    i = ++(*n);
    while ((i != 0) && (item.weight < heap[(i - 1) / 2].weight))
    {
        heap[i] = heap[(i - 1) / 2];
        i--;
        i /= 2;
    }
    heap[i] = item;
}

edge pop(int *n, edge heap[])
{
    int parent, child;
    edge item, tmp;
    item = heap[0];
    tmp = heap[(*n)--];
    parent = 0;
    child = 1;
    while (child <= *n)
    {
        if (child < *n && heap[child].weight > heap[child + 1].weight)
            child++;
        if (tmp.weight <= heap[child].weight)
            break;
        heap[parent] = heap[child];
        parent = child;
        child = parent * 2 + 1;
    }
    heap[parent] = tmp;
    return item;
}

void weightUnion(int a, int b, int sets[])
{
    int parentA = collapsingFind(a, sets);
    int parentB = collapsingFind(b, sets);
    if (parentA == parentB)
        return;

    if (sets[parentA] > sets[parentB]) // B > A
    {
        sets[parentB] += sets[parentA];
        sets[parentA] = parentB;
    }
    else
    {
        sets[parentA] += sets[parentB];
        sets[parentB] = parentA;
    }
}

int collapsingFind(int i, int sets[])
{
    int root;
    for (root = i; sets[root] >= 0; root = sets[root])
        ;
    while (i != root)
    {
        int parent = sets[i];
        sets[i] = root;
        i = parent;
    }
    return root;
}
