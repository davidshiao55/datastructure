#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

// number of time to test false positive
#define TEST 100000
// number of bits in bloom filter
#define M 10001
// number of initial updates
#define U 1000

// number of hash functions
int H;

bool dataExist(int bf[], int size, int key);
void addData(int bf[], int size, int key);

unsigned int hash(int i, unsigned int key, int size);
unsigned int hash1(unsigned int key, int size);
unsigned int hash2(unsigned int key, int size);

int main()
{
    double tH = log(2) * (double)M / (double)U;
    int rtH = round(tH);
    int start = rtH - 4 > 0 ? rtH - 4 : 1;
    int finish = rtH + 4;
    int range = finish - start;
    printf("m = %d u = %d h = %d ~ %d test %d times\n", M, U, start, finish, TEST);
    printf("theory min h = %.3lf\n", tH);

    srand(time(NULL));

    for (H = start; H <= finish; H++)
    {
        unsigned int list[U];
        int bf[M] = {0};
        double p;
        double tP;

        for (int i = 0; i < U; i++)
        {
            int rNum = rand();
            addData(bf, M, rNum);
            list[i] = rNum;
        }

        // generate random string to test collision
        // if(collision) if(not on the list) false positive++
        int falsePositive = 0;
        for (int i = 0; i < TEST; i++)
        {
            int tNum = rand();
            if (dataExist(bf, M, tNum))
            {
                int j = 0;
                for (; j < U; j++)
                {
                    if (tNum == list[j])
                        break;
                }
                // not on the list
                if (j == U)
                {
                    falsePositive++;
                }
            }
        }

        p = (double)falsePositive / TEST * 100;
        double t = 1 - (double)1 / M;
        tP = pow(t, U) * pow(1 - pow(t, U * H), H) * 100;
        printf("h = %d\n", H);
        printf("theory false positve rate : %.3lf%%\n", tP);
        printf("test false positive rate : %.3lf%%\n", p);
    }
    return 0;
}

void addData(int bf[], int size, int key)
{
    for (int i = 0; i < H; i++)
    {
        bf[hash(i, key, size)] = 1;
    }
}

bool dataExist(int bf[], int size, int key)
{
    for (int i = 0; i < H; i++)
    {
        if (!bf[hash(i, key, size)])
            return false;
    }
    return true;
}

// generate hash function using hash1 & hash2
unsigned int hash(int i, unsigned int key, int size)
{
    return (hash1(key, size) + hash2(key, size) * i) % size;
}

unsigned int hash1(unsigned int key, int size)
{
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % size;
}

unsigned int hash2(unsigned int key, int size)
{
    key ^= (key << 13);
    key ^= (key >> 17);
    key ^= (key << 5);
    return key % size;
}