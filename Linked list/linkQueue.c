#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUES 10

typedef struct element
{
    int data;
} element;

typedef struct queue
{
    element data;
    queue *link;
} queue;
// assume front[0 <= i < MAX_QUEUES] = NULL;

void addq(int i, element item);
element deleteq(int i);

queue *front[MAX_QUEUES], *rear[MAX_QUEUES];

int main()
{
    return 0;
}

void addq(int i, element item)
{
    queue *tmp = (queue *)malloc(sizeof(queue));
    tmp->data = item;
    tmp->link = NULL;
    if (front[i])
    {
        rear[i]->link = tmp;
    }
    else
    {
        front[i] = tmp;
    }
    rear[i] = tmp;
}

element deleteq(int i)
{
    queue *tmp = front[i];
    element item;

    if (!tmp)
    {
        perror("queue empty");
    }
    item = tmp->data;
    front[i] = tmp->link;
    free(tmp);
    return item;
}
