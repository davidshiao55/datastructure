#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fRecur(int n);
int fIter(int n);

int main()
{
    int n;
    int x;
    char com[20];

    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%s %d", com, &x);
        if (!strcmp(com, "recursive"))
        {
            printf("%d\n", fRecur(x));
        }
        else if (!strcmp(com, "iterative"))
        {
            printf("%d\n", fIter(x));
        }
    }
    return 0;
}

int fRecur(int n)
{
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    return fRecur(n - 1) + fRecur(n - 2);
}
int fIter(int n)
{
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;

    int f0 = 0, f1 = 1, s;
    int tmp;
    for (int i = 2; i <= n; i++)
    {
        s = f0 + f1;
        f0 = f1;
        f1 = s;
    }
    return s;
}