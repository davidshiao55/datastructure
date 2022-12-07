#include <stdio.h>
#include <stdlib.h>

typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
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

typedef struct queue
{
    int node;
    struct queue *link;
} queue;

queue *front = NULL;
queue *rear = NULL;

void push(int node);
int pop();

AdjListNode *newAdjListNode(int dest, int weight);
graph *creatGraph(int v);
void addEdge(graph *g, int start, int end, int weight);

int main()
{
    int v, root;
    scanf("%d", &v);
    graph *g = creatGraph(v);

    for (int i = 0; i < v - 1; i++)
    {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        addEdge(g, src - 1, dest - 1, weight);
    }

    int shortestPath[v];
    for (int i = 0; i < v; i++)
        shortestPath[i] = 0;

    scanf("%d", &root);
    push(root - 1);

    while (front)
    {
        int curr = pop();
        AdjListNode *l = g->list[curr].head;
        while (l)
        {
            if (shortestPath[l->dest] == 0)
            {
                shortestPath[l->dest] = shortestPath[curr] + l->weight;
                push(l->dest);
            }
            l = l->next;
        }
    }
    shortestPath[root - 1] = 0;

    for (int i = 0; i < v; i++)
    {
        printf("%d %d\n", i + 1, shortestPath[i]);
    }
    return 0;
}

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

void push(int node)
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->node = node;
    if (!front)
    {
        front = q;
        rear = q;
        q->link = NULL;
    }
    else
    {
        rear->link = q;
        rear = q;
        q->link = NULL;
    }
}
int pop()
{
    if (!front)
    {
        printf("queue empty");
        exit(EXIT_FAILURE);
    }
    queue *tmp = front;
    int node = tmp->node;
    front = front->link;
    free(tmp);
    return node;
}
