#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NAME 50
#define COMPARE(A, B) (A > B) ? 1 : ((A == B) ? 0 : -1)

typedef struct polyNode
{
    float coef;
    int expon;
    polyNode *link;
} polyNode;

typedef struct polyNomial
{
    polyNode *header;
    char name[MAX_NAME];
    polyNomial *link;
} polyNomial;

void attachH(float coef, int expon, polyNode **first, polyNode **rear);
polyNode *paddH(polyNode *a, polyNode *b);
polyNode *psubH(polyNode *a, polyNode *b);
void printPolynomialH(polyNode *first);
void erase(polyNode **first);
polyNode *pread(float coef[], int expon[], int n);
polyNode *getNode();
void retNode(polyNode *tmp);
void addPoly(polyNomial **first, polyNode *head, char name[MAX_NAME]);
void deletePoly(polyNomial **first, polyNomial *p);
polyNomial *findPolynomial(polyNomial *first, char *name);
void perase(polyNode **p);
polyNode *pmult(polyNode *a, polyNode *b);
float peval(polyNomial *p, float x);
float myPow(int expon, float x);

polyNode *avail = NULL;

int main()
{
    int n;
    char com[20];
    polyNomial *polyList = NULL;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%s", com);
        if (!strcmp(com, "pread"))
        {
            char name[MAX_NAME];
            polyNode *tmp;
            int j;
            scanf("%s", name);
            scanf("%d", &j);
            float coef[j];
            int expon[j];
            for (int k = 0; k < j; k++)
            {
                scanf("%f %d", &coef[k], &expon[k]);
            }
            tmp = pread(coef, expon, j);
            addPoly(&polyList, tmp, name);
            printf("ok\n");
        }

        if (!strcmp(com, "pwrite"))
        {
            char name[MAX_NAME];
            scanf("%s", &name);
            printPolynomialH(findPolynomial(polyList, name)->header);
        }

        if (!strcmp(com, "padd"))
        {
            char name[MAX_NAME];
            char poly1[MAX_NAME];
            char poly2[MAX_NAME];
            scanf("%s %s %s", name, poly1, poly2);
            polyNode *tmp = paddH(findPolynomial(polyList, poly1)->header,
                                  findPolynomial(polyList, poly2)->header);
            addPoly(&polyList, tmp, name);
            printf("added\n");
        }

        if (!strcmp(com, "psub"))
        {
            char name[MAX_NAME];
            char poly1[MAX_NAME];
            char poly2[MAX_NAME];
            scanf("%s %s %s", name, poly1, poly2);
            polyNode *tmp = psubH(findPolynomial(polyList, poly1)->header,
                                  findPolynomial(polyList, poly2)->header);
            addPoly(&polyList, tmp, name);
            printf("subtracted\n");
        }

        if (!strcmp(com, "pmult"))
        {
            char name[MAX_NAME];
            char poly1[MAX_NAME];
            char poly2[MAX_NAME];
            scanf("%s %s %s", name, poly1, poly2);
            polyNode *tmp = pmult(findPolynomial(polyList, poly1)->header,
                                  findPolynomial(polyList, poly2)->header);
            addPoly(&polyList, tmp, name);
            printf("multiplied\n");
        }

        if (!strcmp(com, "perase"))
        {
            char name[MAX_NAME];
            scanf("%s", name);
            polyNomial *tmp = findPolynomial(polyList, name);
            polyNode *tmpNode = tmp->header;
            perase(&tmpNode);
            deletePoly(&polyList, tmp);
            printf("erased\n");
        }

        if (!strcmp(com, "eval"))
        {
            char name[MAX_NAME];
            float x;
            scanf("%s %f", name, &x);
            printf("%.2f\n", peval(findPolynomial(polyList, name), x));
        }
    }
    return 0;
}

float myPow(int expon, float x)
{
    return (float)pow((double)x, (double)expon);
}

float peval(polyNomial *p, float x)
{
    polyNode *tmp = p->header, *startp = p->header;
    float sum = 0;
    for (tmp = tmp->link; tmp != startp; tmp = tmp->link)
    {
        sum += tmp->coef * myPow(tmp->expon, x);
    }
    sum = (float)(floor(sum * 100) / 100);
    return sum;
}

polyNode *pread(float coef[], int expon[], int n)
{
    polyNode *_a, *a = (polyNode *)malloc(sizeof(polyNode));
    a->coef = 0;
    a->expon = -1;
    a->link = a;
    _a = a;
    for (int i = 0; i < n; i++)
    {
        attachH(coef[i], expon[i], &a, &_a);
    }
    return a;
}

polyNode *pmult(polyNode *a, polyNode *b)
{
    polyNode *startA, *startB, *c, *_c;
    startA = a;
    startB = b;
    a = a->link;
    b = b->link;
    c = (polyNode *)malloc(sizeof(polyNode));
    c->expon = -1;
    c->link = c;
    _c = c;
    while (a != startA)
    {
        while (b != startB)
        {
            float coef = a->coef * b->coef;
            int expon = a->expon + b->expon;
            polyNode *newnode, *tmp = c, *prev = c;
            for (tmp = tmp->link; tmp != c; tmp = tmp->link, prev = prev->link)
            {
                // new node > current node : insert between prev & current
                if (expon > tmp->expon)
                {
                    newnode = (polyNode *)malloc(sizeof(polyNode));
                    newnode->coef = coef;
                    newnode->expon = expon;
                    prev->link = newnode;
                    newnode->link = tmp;
                    break;
                }
                // new node == currenet node : combind two node
                else if (expon == tmp->expon)
                {
                    tmp->coef += coef;
                    break;
                }
            }
            if (tmp == c)
            {
                attachH(coef, expon, &c, &_c);
            }
            b = b->link;
        }
        a = a->link;
        b = b->link;
    }
    return c;
}

polyNode *paddH(polyNode *a, polyNode *b)
{
    polyNode *startA, *c, *_c;
    bool flag = false;
    startA = a;
    a = a->link;
    b = b->link;
    c = (polyNode *)malloc(sizeof(polyNode));
    c->expon = -1;
    _c = c;
    while (!flag)
    {
        switch (COMPARE(a->expon, b->expon))
        {
        case 1:
            attachH(a->coef, a->expon, &c, &_c);
            a = a->link;
            break;
        case 0:
            if (startA == a)
            {
                flag = true;
                break;
            }
            attachH(a->coef + b->coef, a->expon, &c, &_c);
            a = a->link;
            b = b->link;
            break;
        case -1:
            attachH(b->coef, b->expon, &c, &_c);
            b = b->link;
            break;
        }
    }
    return c;
}

polyNode *psubH(polyNode *a, polyNode *b)
{
    polyNode *startA, *c, *_c;
    bool flag = false;
    startA = a;
    a = a->link;
    b = b->link;
    c = (polyNode *)malloc(sizeof(polyNode));
    c->expon = -1;
    _c = c;
    while (!flag)
    {
        switch (COMPARE(a->expon, b->expon))
        {
        case 1:
            attachH(a->coef, a->expon, &c, &_c);
            a = a->link;
            break;
        case 0:
            if (startA == a)
            {
                flag = true;
                break;
            }
            attachH(a->coef - b->coef, a->expon, &c, &_c);
            a = a->link;
            b = b->link;
            break;
        case -1:
            attachH(-b->coef, b->expon, &c, &_c);
            b = b->link;
            break;
        }
    }
    return c;
}

void attachH(float coef, int expon, polyNode **first, polyNode **rear)
{
    polyNode *tmp = getNode();
    tmp->coef = coef;
    tmp->expon = expon;
    tmp->link = NULL;
    if (*first)
        (*rear)->link = tmp;
    else
        *first = tmp;
    *rear = tmp;
    (*rear)->link = *first;
}

void perase(polyNode **p)
{
    polyNode *tmp, *startp = *p;
    do
    {
        tmp = *p;
        *p = (*p)->link;
        retNode(tmp);
    } while (*p != startp);
}

void erase(polyNode **first)
{
    polyNode *tmp;
    while (*first)
    {
        tmp = *first;
        *first = (*first)->link;
        free(tmp);
    }
}

void printPolynomialH(polyNode *first)
{
    polyNode *start = first;
    for (first = first->link; first != start; first = first->link)
    {
        printf("%.0fx^%d", first->coef, first->expon);
        if (first->link != first && first->link->coef > 0)
            printf("+");
    }
    printf("\n");
}

polyNode *getNode()
{
    polyNode *tmp;
    if (avail)
    {
        tmp = avail;
        avail = avail->link;
    }
    else
    {
        tmp = (polyNode *)malloc(sizeof(polyNode));
    }
    return tmp;
}

void retNode(polyNode *tmp)
{
    tmp->link = avail;
    avail = tmp;
}

void addPoly(polyNomial **first, polyNode *head, char name[MAX_NAME])
{
    polyNomial *tmp = (polyNomial *)malloc(sizeof(polyNomial));
    tmp->header = head;
    strcpy(tmp->name, name);
    if (*first)
    {
        tmp->link = *first;
    }
    else
    {
        tmp->link = NULL;
    }
    *first = tmp;
}

polyNomial *findPolynomial(polyNomial *first, char *name)
{
    while (first)
    {
        if (!strcmp(name, first->name))
            break;
        first = first->link;
    }
    return first;
}

void deletePoly(polyNomial **first, polyNomial *p)
{
    if (*first == p)
        *first = (*first)->link;
    else
    {
        polyNomial *trail = *first;
        while (trail->link != p)
        {
            trail = trail->link;
        }
        trail->link = p->link;
    }
    free(p);
}
