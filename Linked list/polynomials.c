#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COMPARE(A, B) (A > B) ? 1 : ((A == B) ? 0 : -1)

typedef struct polyNode
{
    float coef;
    int expon;
    polyNode *link;
} polyNode;

void attach(float coef, int expon, polyNode **first, polyNode **rear);
void printPolynomial(polyNode *first);
void erase(polyNode **first);
polyNode *padd(polyNode *a, polyNode *b);
polyNode *paddH(polyNode *a, polyNode *b);
void printPolynomialH(polyNode *first);
void attachH(float coef, int expon, polyNode **first, polyNode **rear);

int main()
{
    polyNode *a = NULL;
    polyNode *_a = a;
    attachH(0, -1, &a, &_a);
    attachH(5, 4, &a, &_a);
    attachH(5, 3, &a, &_a);

    polyNode *b = NULL;
    polyNode *_b = b;
    attachH(0, -1, &b, &_b);
    attachH(5, 4, &b, &_b);
    attachH(5, 3, &b, &_b);

    polyNode *c = paddH(a, b);
    // printPolynomial(c);
    printPolynomialH(c);
    return 0;
}

// add polynomials with circular linked list and header node
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
            attach(a->coef, a->expon, &c, &_c);
            a = a->link;
            break;
        case 0:
            if (startA == a)
            {
                flag = true;
                break;
            }
            attach(a->coef + b->coef, a->expon, &c, &_c);
            a = a->link;
            b = b->link;
            break;
        case -1:
            attach(b->coef, b->expon, &c, &_c);
            b = b->link;
            break;
        }
    }
    _c->link = c;
    return c;
}

polyNode *padd(polyNode *a, polyNode *b)
{
    polyNode *c, *rear, *tmp;
    c = NULL;
    rear = c;
    while (a && b)
    {
        switch (COMPARE(a->expon, b->expon))
        {
        case 1:
            attach(a->coef, a->expon, &c, &rear);
            a = a->link;
            break;
        case 0:
            attach(a->coef + b->coef, a->expon, &c, &rear);
            a = a->link;
            b = b->link;
            break;
        case -1:
            attach(b->coef, b->expon, &c, &rear);
            b = b->link;
            break;
        }
    }
    for (; a; a = a->link)
        attach(a->coef, a->expon, &c, &rear);
    for (; b; b = b->link)
        attach(b->coef, b->expon, &c, &rear);
    rear->link = NULL;
    return c;
}

void attach(float coef, int expon, polyNode **first, polyNode **rear)
{
    polyNode *tmp = (polyNode *)malloc(sizeof(polyNode));
    tmp->coef = coef;
    tmp->expon = expon;
    tmp->link = NULL;
    if (*first)
        (*rear)->link = tmp;
    else
        *first = tmp;
    *rear = tmp;
}

void attachH(float coef, int expon, polyNode **first, polyNode **rear)
{
    polyNode *tmp = (polyNode *)malloc(sizeof(polyNode));
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

void printPolynomial(polyNode *first)
{
    for (; first; first = first->link)
    {
        printf("%.0fx^%d ", first->coef, first->expon);
    }
}

void printPolynomialH(polyNode *first)
{
    polyNode *start = first;
    for (first = first->link; first != start; first = first->link)
    {
        printf("%.0fx^%d ", first->coef, first->expon);
    }
}