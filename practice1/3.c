#include <stdio.h>
#include <stdlib.h>

#define MAX_TERM 101
#define MAX_COL 10000

typedef struct term
{
    int col;
    int row;
    int value;
} term;

void fastTranspose(term a[], term b[]);

int main()
{
    int r, c, t;
    scanf("%d %d %d", &r, &c, &t);
    term *a = (term *)malloc((t + 1) * sizeof(term));
    term *b = (term *)malloc((t + 1) * sizeof(term));

    a[0].row = r;
    a[0].col = c;
    a[0].value = t;

    for (int i = 1; i <= t; i++)
    {
        int tmpR, tmpC, tmpV;
        scanf("%d %d %d", &tmpR, &tmpC, &tmpV);
        a[i].row = tmpR;
        a[i].col = tmpC;
        a[i].value = tmpV;
    }

    fastTranspose(a, b);

    for (int i = 0; i <= t; i++)
        printf("%d %d %d\n", b[i].row, b[i].col, b[i].value);

    return 0;
}

void fastTranspose(term a[], term b[])
{
    int pos[MAX_COL] = {0};

    b[0].col = a[0].row;
    b[0].row = a[0].col;
    b[0].value = a[0].value;

    pos[0] = 1;
    for (int i = 1; i <= a[0].value; i++)
    {
        pos[a[i].col + 1]++;
    }
    for (int i = 1; i <= a[0].col; i++)
    {
        pos[i] += pos[i - 1];
    }
    for (int i = 1; i <= a[0].value; i++)
    {
        int j = pos[a[i].col]++;
        b[j].row = a[i].col;
        b[j].col = a[i].row;
        b[j].value = a[i].value;
    }
}