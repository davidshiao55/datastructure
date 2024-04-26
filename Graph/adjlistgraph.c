#include <stdlib.h>

typedef struct AdjListNode
{
    int dest;
    int weight;
    AdjListNode *next;
} AdjListNode;

typedef struct AdjList
{
    struct AdjListNode *head;
} AdjList;

typedef struct graph
{
    int v;
    AdjList *list;
} graph;

AdjListNode *newAdjListNode(int dest, int weight);
graph *creatGraph(int v);
void addEdge(graph *g, int start, int end, int weight);

graph *creatGraph(int v)
{
    graph *g = (graph *)malloc(sizeof(graph));
    g->v = v;
    g->list = (AdjList *)malloc(sizeof(AdjList) * v);

    for (int i = 0; i < v; i++)
        g->list[i].head = NULL;
    return g;
}

AdjListNode *newAdjListNode(int dest, int weight)
{
    AdjListNode *node = (AdjListNode *)malloc(sizeof(AdjListNode));
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

void addEdge(graph *g, int start, int end, int weight)
{
    AdjListNode *newNode = newAdjListNode(end, weight);

    if (!g->list[start].head)
    {
        g->list[start].head = newNode;
    }
    else
    {
        AdjListNode *tmp = g->list[start].head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = newNode;
    }

    newNode = newAdjListNode(start, weight);

    if (!g->list[end].head)
    {
        g->list[end].head = newNode;
    }
    else
    {
        AdjListNode *tmp = g->list[end].head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = newNode;
    }
}
