//  use too much memory, fail testcase3 FUCK BRO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int value;
} node;

typedef struct edge
{
    int src;
    int dest;
    int weight;
} edge;

typedef struct graph
{
    int v;
    int e;
    edge **edgeList;
} graph;

void push(edge item, int *n, int e, edge heap[]);
edge pop(int *n, edge heap[]);

void weightUnion(int a, int b, int sets[]);
int collapsingFind(int i, int sets[]);

graph *creatGraph(int v, int e);
void addEdge(graph *g, int start, int end, int weight);
graph *minSpanningTree(graph *g); // use pointer save space

int main()
{
    int v, e;
    double cost = 0;
    scanf("%d %d", &v, &e);
    graph *g = creatGraph(v, e);

    for (int i = 0; i < e; i++)
    {
        int start, end, weight;
        scanf("%d %d %d", &start, &end, &weight);
        addEdge(g, start, end, weight);
    }

    graph *mst = minSpanningTree(g);
    for (int i = 0; i < mst->e; i++)
    {
        cost += mst->edgeList[i]->weight;
    }
    printf("%.0lf\n", cost);
    return 0;
}

graph *minSpanningTree(graph *g)
{
    graph *mst = creatGraph(g->v, g->v - 1);
    edge heap[g->e];
    int n = -1;

    for (int i = 0; i < g->e; i++)
        push(*(g->edgeList[i]), &n, g->e, heap);

    int sets[g->v];
    for (int i = 0; i < g->v; i++)
        sets[i] = -1;

    while (n != -1)
    {
        edge minEdge = pop(&n, heap);
        if (collapsingFind(minEdge.dest, sets) == collapsingFind(minEdge.src, sets))
            continue;
        addEdge(mst, minEdge.src, minEdge.dest, minEdge.weight);
        weightUnion(minEdge.src, minEdge.dest, sets);
    }
    return mst;
}

graph *creatGraph(int v, int e)
{
    graph *g = (graph *)malloc(sizeof(graph));
    g->v = v;
    g->e = e;
    g->edgeList = (edge **)malloc(sizeof(edge *) * e);
    for (int i = 0; i < e; i++)
    {
        g->edgeList[i] = NULL;
    }
    return g;
}

void addEdge(graph *g, int start, int end, int weight)
{
    edge *newEdge = (edge *)malloc(sizeof(edge));
    newEdge->src = start;
    newEdge->dest = end;
    newEdge->weight = weight;
    int i = 0;
    while (g->edgeList[i])
        i++;
    g->edgeList[i] = newEdge;
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
