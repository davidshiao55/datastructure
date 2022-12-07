#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue
{
    int node;
    struct queue *link;
} queue;

queue *front = NULL;
queue *rear = NULL;

void bfs(int m, int t, int adj[][m], bool visited[]);
void push(int node);
int pop();

int main()
{
    int rounds;
    scanf("%d", &rounds);
    for (int i = 0; i < rounds; i++)
    {
        int m, t;
        scanf("%d %d", &m, &t);
        int adj[m][m];
        bool visited[m];
        for (int j = 0; j < m; j++)
        {
            visited[j] = false;
            for (int k = 0; k < m; k++)
                scanf("%d", &adj[j][k]);
        }
        bfs(m, t, adj, visited);
        printf("\n");
    }
    return 0;
}

void bfs(int m, int t, int adj[][m], bool visited[])
{
    visited[t] = true;
    push(t);
    while (front)
    {
        int curr = pop();
        printf("%d ", curr);
        for (int i = 0; i < m; i++)
        {
            if (adj[curr][i] == 1 && !visited[i])
            {
                visited[i] = true;
                push(i);
            }
        }
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
