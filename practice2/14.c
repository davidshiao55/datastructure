#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void dfs(int m, int t, int adj[][m], bool visited[]);

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
        dfs(m, t, adj, visited);
        printf("\n");
    }
    return 0;
}

void dfs(int m, int t, int adj[][m], bool visited[])
{
    if (visited[t])
        return;
    visited[t] = true;
    printf("%d ", t);
    for (int i = 0; i < m; i++)
    {
        if (adj[t][i] == 1)
            dfs(m, i, adj, visited);
    }
}