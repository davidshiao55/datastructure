#include <stdio.h>
#include <stdlib.h>
#define MAXSTACK 100

struct disk
{
    int d;
};

struct disk rod[3][MAXSTACK];
int top[3] = {-1, -1, -1};
int isEmpty(int n);
void push(struct disk, int n);
struct disk pop(int n);
void move(int n, int x, int y, int z);

int main()
{
    int numDisk;
    scanf("%d", &numDisk);
    for (int i = numDisk; i > 0; i--)
    {
        struct disk newDisk;
        newDisk.d = i;
        push(newDisk, 0);
    }
    move(numDisk, 0, 1, 2);
    return 0;
}

void move(int n, int x, int y, int z)
{
    if (n == 1)
    {
        printf("%d %d %d\n", rod[x][top[x]].d, x + 1, z + 1);
        push(pop(x), z);
        return;
    }
    move(n - 1, x, z, y);
    printf("%d %d %d\n", rod[x][top[x]].d, x + 1, z + 1);
    push(pop(x), z);
    move(n - 1, y, x, z);
}

int isEmpty(int n)
{
    if (top[n] == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void push(struct disk data, int n)
{
    if (top[n] >= MAXSTACK)
    {
        // printf("堆疊已滿,無法再加入\n");
    }
    else
    {
        top[n]++;
        rod[n][top[n]] = data;
    }
}

struct disk pop(int n)
{
    struct disk data;
    if (isEmpty(n))
    {
        perror("堆疊已空\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        data = rod[n][top[n]];
        top[n]--;
        return data;
    }
}