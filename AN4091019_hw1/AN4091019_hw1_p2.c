#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENT 200
#define HEAP_EMPTY(n) n == -1
#define HEAP_FULL(n) (n == MAX_ELEMENT - 1)

typedef struct element
{
    int value;
} element;

void push(element item, int *n);
element pop(int *n);
void change(int x, int y, int *n);
element heap[MAX_ELEMENT];
int n = -1;

int main()
{
    char com[20];
    int x, y;
    element *ptr[MAX_ELEMENT];
    for (int i = 0; i < MAX_ELEMENT; i++)
        ptr[i] = &heap[i];
    while (1)
    {
        scanf("%s", com);
        if (!strcmp(com, "insert"))
        {
            scanf("%d", &x);
            element e = {x};
            push(e, &n);
        }

        if (!strcmp(com, "remove"))
        {
            if (HEAP_EMPTY(n))
            {
                printf("empty\n");
            }
            else
                printf("%d\n", pop(&n).value);
        }

        if (!strcmp(com, "change"))
        {
            scanf("%d,%d", &x, &y);
            change(x, y, &n);
        }

        if (!strcmp(com, "quit"))
        {
            break;
        }
    }
    return 0;
}

void change(int x, int y, int *n)
{
    if (x > *n || x < 0)
    {
        printf("out of range\n");
        return;
    }
    int pos = x;
    element tmp = heap[x];
    tmp.value = y;
    if (y > heap[x].value) //往下沉
    {
        int parent, child;
        parent = x;
        child = 2 * x + 1;
        while (child <= *n)
        {
            if (child < *n && heap[child].value > heap[child + 1].value)
                child++;
            if (tmp.value <= heap[child].value)
                break;
            heap[parent] = heap[child];
            parent = child;
            child = parent * 2 + 1;
        }
        pos = parent;
    }
    else
    {
        int i = x;
        while ((i != 0) && (tmp.value < heap[(i - 1) / 2].value))
        {
            heap[i] = heap[(i - 1) / 2];
            i--;
            i /= 2;
        }
        pos = i;
    }
    heap[pos] = tmp;
}

void push(element item, int *n)
{
    int i;
    if (HEAP_FULL(*n))
    {
        printf("full. \n");
        return;
    }
    i = ++(*n);
    while ((i != 0) && (item.value < heap[(i - 1) / 2].value))
    {
        heap[i] = heap[(i - 1) / 2];
        i--;
        i /= 2;
    }
    heap[i] = item;
}

element pop(int *n)
{
    int parent, child;
    element item, tmp;
    item = heap[0];
    tmp = heap[(*n)--];
    parent = 0;
    child = 1;
    while (child <= *n)
    {
        if (child < *n && heap[child].value > heap[child + 1].value)
            child++;
        if (tmp.value <= heap[child].value)
            break;
        heap[parent] = heap[child];
        parent = child;
        child = parent * 2 + 1;
    }
    heap[parent] = tmp;
    return item;
}
