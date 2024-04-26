#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n = 10, m = 10;
    char com;
    int ibug, jbug;
    int stepCount = 0;
    int untouch;
    int imove[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int jmove[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    srand(time(NULL));

    scanf("%c", &com);

    if (com == 'a')
    {
        scanf("%d %d", &n, &m);
        ibug = n / 2;
        jbug = m / 2;
    }
    else if (com == 'b')
    {
        int t;
        scanf("%d", &t);
        if (t == 1)
        {
            n = 15;
            m = 15;
            ibug = 10;
            jbug = 10;
        }
        else if (t == 2)
        {
            n = 39;
            m = 19;
            ibug = 1;
            jbug = 1;
        }
    }

    int **map = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
    {
        map[i] = (int *)malloc(sizeof(int) * m);
    }

    untouch = n * m;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            map[i][j] = 0;
        }
    }
    // map[ibug][jbug] = 1; does the first step count?

    int r;
    while (untouch > 0 && stepCount < 50000)
    {
        r = rand() % 8;
        if (ibug + imove[r] >= n || ibug + imove[r] < 0 || jbug + jmove[r] >= m || jbug + jmove[r] < 0)
        {
            continue;
        }
        ibug = ibug + imove[r];
        jbug = jbug + jmove[r];

        if (map[ibug][jbug] == 0)
        {
            untouch--;
        }
        map[ibug][jbug]++;
        stepCount++;
    }
    int sum = 0;
    printf("%d\n", stepCount);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%4d", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}