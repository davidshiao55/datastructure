#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_STACK 1000000

typedef struct step
{
    int x;
    int y;
} step;

step stack[MAX_STACK];
int top = -1;
bool isEmpty();
struct step pop();
struct step prevStep();
void push(struct step s);

bool isSafe(int **maze, int x, int y, int n, int m);
bool solveMazeUntil(int **maze, int x, int y, int n, int m);

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);

    int **maze = (int **)malloc(sizeof(int *) * n);

    for (int i = 0; i < n; i++)
    {
        maze[i] = (int *)malloc(sizeof(int) * m);

        for (int j = 0; j < m; j++)
        {
            scanf("%d", &maze[i][j]);
        }
    }

    if (solveMazeUntil(maze, 0, 0, n, m))
    {
        for (int i = 0; i <= top; i++)
        {
            printf("%d %d\n", stack[i].x, stack[i].y);
        }
    }
    else
    {
        printf("no answer\n");
    }

    return 0;
}

bool isSafe(int **maze, int x, int y, int n, int m)
{
    if (x < n && x >= 0 && y < m && y >= 0 && maze[x][y] == 0 && (x != prevStep().x || y != prevStep().y))
    {
        return true;
    }
    return false;
}

bool solveMazeUntil(int **maze, int x, int y, int n, int m)
{
    if (x == n - 1 && y == m - 1)
    {
        struct step final = {x, y};
        push(final);
        return true;
    }

    if (isSafe(maze, x, y, n, m))
    {
        struct step s = {x, y};
        push(s);

        if (solveMazeUntil(maze, x + 1, y + 1, n, m))
        {
            return true;
        }
        if (solveMazeUntil(maze, x, y + 1, n, m))
        {
            return true;
        }
        if (solveMazeUntil(maze, x + 1, y, n, m))
        {
            return true;
        }

        if (solveMazeUntil(maze, x - 1, y + 1, n, m))
        {
            return true;
        }
        if (solveMazeUntil(maze, x, y - 1, n, m))
        {
            return true;
        }
        if (solveMazeUntil(maze, x - 1, y - 1, n, m))
        {
            return true;
        }

        if (solveMazeUntil(maze, x - 1, y, n, m))
        {
            return true;
        }
        if (solveMazeUntil(maze, x + 1, y - 1, n, m))
        {
            return true;
        }
        pop();
        return false;
    }
}

bool isEmpty()
{
    if (top == -1)
    {
        return true;
    }
    return false;
}
struct step pop()
{
    if (isEmpty())
    {
        perror("堆疊以空");
        exit(EXIT_FAILURE);
    }
    else
    {
        return stack[top--];
    }
}
void push(struct step s)
{
    if (top >= MAX_STACK - 1)
    {
        perror("堆疊已滿");
        exit(EXIT_FAILURE);
    }
    else
    {
        stack[++top] = s;
    }
}

struct step prevStep()
{
    if (top > 0)
    {
        return stack[top - 1];
    }
    else
    {
        struct step z = {-1, -1};
        return z;
    }
}
