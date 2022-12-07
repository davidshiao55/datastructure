#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void heightUnion(int a, int b, int sets[], int n);
void weightUnion(int a, int b, int sets[], int n);
int collapsingFind(int i, int sets[]);
int find(int a, int sets[]);

int main()
{
    return 0;
}

void weightUnion(int a, int b, int sets[], int n)
{
    int parentA = collapsingFind(a, sets);
    int parentB = collapsingFind(b, sets);
    if (parentA == parentB)
        return;

    if (sets[parentA] > sets[parentB]) // B > A
    {
        sets[parentA] = parentB;
        sets[parentB] += sets[parentA];
    }
    else
    {
        sets[parentB] = parentA;
        sets[parentA] += sets[parentB];
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

void heightUnion(int a, int b, int sets[], int n)
{
    int parentA = find(a, sets);
    int parentB = find(b, sets);
    if (parentA == parentB)
        return;
    if (sets[parentA] > sets[parentB]) // b is greater
    {
        sets[parentA] = parentB;
        sets[parentB]--;
    }
    else
    {
        sets[parentB] = parentA;
        sets[parentA]--;
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
