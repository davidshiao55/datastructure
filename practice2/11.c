#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool same(int a, int b, int sets[], int n);
void heightUnion(int a, int b, int sets[], int n);
int find(int a, int sets[]);

int main()
{
    int rounds;
    int n, ops;
    char com[20];

    scanf("%d", &rounds);
    for (int i = 0; i < rounds; i++)
    {
        scanf("%d %d", &n, &ops);
        int sets[n];
        for (int j = 0; j < n; j++)
        {
            sets[j] = -1;
        }
        for (int j = 0; j < ops; j++)
        {
            scanf("%s", com);
            if (!strcmp(com, "union"))
            {
                int a, b;
                scanf("%d %d", &a, &b);
                heightUnion(a, b, sets, n);
            }
            else if (!strcmp(com, "find"))
            {
                int a;
                scanf("%d", &a);
                printf("%d\n", find(a, sets));
            }
            else if (!strcmp(com, "same"))
            {
                int a, b;
                scanf("%d %d", &a, &b);
                printf(same(a, b, sets, n) ? "true\n" : "false\n");
            }
        }
    }
    return 0;
}

bool same(int a, int b, int sets[], int n)
{
    return (find(a, sets) == find(b, sets));
}

void heightUnion(int a, int b, int sets[], int n)
{
    int parentA = find(a, sets);
    int parentB = find(b, sets);
    if (parentA == parentB)
        return;
    if (sets[parentA] > sets[parentB]) // b is greater
    {
        sets[parentA] = parentB;
    }
    else
    {
        if (sets[parentA] == sets[parentB])
            sets[parentA]--;
        sets[parentB] = parentA;
    }
}

int find(int a, int sets[])
{
    while (sets[a] >= 0)
    {
        a = sets[a];
    }
    return a;
}
