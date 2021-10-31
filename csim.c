// Haoyuan Tian, 519021911174

#include "cachelab.h"
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

typedef struct
{
    int valid;
    long address;
    int time;
} Cache;

Cache *cache;

int S, E, B;
int hits = 0;
int misses = 0;
int evictions = 0;

int readCache(Cache *cache, char type, long address)
{
    int flag = 0;
    int line = (int)((address / B) % S);
    int pos = 0;
    int minTime = INT_MAX;
    int current = -1;
    int exist = 0;
    int insert = 0;
    for (int i = 0; i < E; i++)
    {
        Cache *element = (cache + line * E + i);
        if (insert == 0 && flag == 0)
        {
            if (!element->valid)
            {
                element->valid = 1;
                element->address = address;
                insert = 1;
                exist = 0;
                flag = 2;
                pos = i;
                if (type == 'M')
                {
                    flag = 3;
                }
            }
            else
            {
                if ((element->address / B) == (address / B))
                {
                    exist = 1;
                    pos = i;
                    flag = 1;
                }
                if (element->time < minTime)
                {
                    minTime = element->time;
                    if (exist == 0)
                    {
                        pos = i;
                    }
                }
            }
        }
        if (element->time > current)
            current = element->time;
    }
    Cache *element = (cache + line * E + pos);
    if (insert == 0 && exist == 0)
    {
        element->address = address;
        flag = 4;
        if (type == 'M')
        {
            flag = 5;
        }
    }
    element->time = current + 1;
    return flag;
}

void printHelp()
{
    printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
}

int main(int argc, char *argv[])
{
    opterr = 0;
    int verbose = 0;
    int para;
    char *name = NULL;
    while ((para = getopt(argc, argv, "s:E:b:t:hv")) != -1)
    {
        switch (para)
        {
        case 's':
            S = (int)pow(2, atoi(optarg));
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            B = (int)pow(2, atoi(optarg));
            break;
        case 't':
            name = optarg;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'h':
            printHelp();
            break;
        }
    }

    FILE *file = fopen(name, "r");

    if (file == NULL)
    {
        printf("NO SUCH FILE!\n");
        return -1;
    }

    cache = (Cache *)malloc(S * E * sizeof(Cache));
    if (cache == NULL)
    {
        printf("MEMORY ALLOCATION FAILED!\n");
        return -1;
    }

    memset(cache, 0, S * E * sizeof(Cache));
    char type;
    int size;
    long address;
    while (!feof(file))
    {
        int tmp = fscanf(file, " %c %lx,%x", &type, &address, &size);
        if (tmp != 3)
            continue;
        if (type != 'I')
        {
            int ret = readCache(cache, type, address);
            char *tmp1 = NULL;
            switch (ret)
            {
            case 1:
                hits++;
                tmp1 = "hit";
                if (type == 'M')
                    hits++;
                break;
            case 2:
                misses++;
                tmp1 = "miss";
                break;

            case 3:
                misses++;
                hits++;
                tmp1 = "miss";
                break;
            case 4:
                misses++;
                evictions++;
                tmp1 = "miss";
                break;
            case 5:
            default:
                misses++;
                evictions++;
                hits++;
                tmp1 = "miss";
                break;
            }
            if (verbose)
            {
                printf("%c %lx,%x %s", type, address, size, tmp1);
                if (ret == 4)
                {
                    printf(" eviction");
                }
                else if (ret == 3)
                {
                    printf(" hit");
                }
                else if (ret == 5)
                {
                    printf(" eviction hit");
                }
                else if (ret == 1 && type == 'M')
                {
                    printf(" hit");
                }

                printf("\n");
            }
        }
    }
    free(cache);
    printSummary(hits, misses, evictions);
}