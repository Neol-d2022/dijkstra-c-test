#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int decreaseSum(unsigned int total, unsigned int level)
{
    return (total * level) - ((level * (level + 1)) >> 1);
}

unsigned int indexOf(unsigned int a, unsigned int b, unsigned int n)
{
    if (a > b)
        return indexOf(b, a, n);
    else if (a < b)
        return decreaseSum(n, a) + (b - a - 1);
    else
        return n;
}

void printMatrix(int *m, unsigned int n)
{
    unsigned int i, j;

    for (i = 0; i < n; i += 1)
    {
        for (j = 0; j < n; j += 1)
        {
            if (i == j)
                printf("  0 ");
            else
            {
                printf("%3d ", m[indexOf(i, j, n)]);
            }
        }
        printf("\n");
    }
}

unsigned int dijkstra(int *m, unsigned int *t, unsigned int n, unsigned int s)
{
    unsigned char *nodeKnown;
    unsigned int *nodeMinDis, *nodeNumHops;
    unsigned int i, j, d, minDis, minI, minJ, minFound, minNumHops, numKnownNodes, numHops;
    int dis;

    nodeKnown = (unsigned char *)malloc(sizeof(*nodeKnown) * n);
    memset(nodeKnown, 0, sizeof(*nodeKnown) * n);
    nodeKnown[s] = 1;
    nodeMinDis = (unsigned int *)malloc(sizeof(*nodeMinDis) * n);
    nodeNumHops = (unsigned int *)malloc(sizeof(*nodeNumHops) * n);
    numKnownNodes = 1;
    nodeMinDis[s] = 0;
    nodeNumHops[s] = 0;
    t[s] = n;

    do
    {
        minDis = (unsigned int)-1;
        minNumHops = (unsigned int)-1;
        minFound = 0;

        for (i = 0; i < n; i += 1)
        {
            if (nodeKnown[i] == 0)
                continue;
            for (j = 0; j < n; j += 1)
            {
                if (nodeKnown[j] == 1)
                    continue;
                if ((dis = m[indexOf(i, j, n)]) >= 0)
                {
                    d = dis + nodeMinDis[i];
                    numHops = nodeNumHops[i] + 1;
                    printf("[DEBUG] #%u-#%u, L:%d, T:%u, H:%u\n", i + 1, j + 1, dis, d, numHops);
                    if (d < minDis || (d == minDis && numHops < minNumHops))
                    {
                        minDis = d;
                        minI = i;
                        minJ = j;
                        minFound += 1;
                        minNumHops = numHops;
                    }
                }
            }
        }

        if (minFound)
        {
            t[minJ] = minI;
            nodeKnown[minJ] = 1;
            nodeMinDis[minJ] = minDis;
            numKnownNodes += 1;
            nodeNumHops[minJ] = minNumHops;
            printf("[DEBUG] #%u-#%u, L:%u, T:%u, H:%u (final)\n", minI + 1, minJ + 1, (unsigned int)m[indexOf(minI, minJ, n)], minDis, minNumHops);
        }

    } while (minFound);

    free(nodeKnown);
    free(nodeMinDis);
    free(nodeNumHops);

    return numKnownNodes;
}

unsigned int dijkstraPath(int *m, unsigned int *t, unsigned int *p, unsigned int *out_dis, unsigned int n, unsigned int source, unsigned int destination)
{
    unsigned int *q;
    unsigned int i, o, parentNode, middleNode, totalDis;

    q = (unsigned int *)malloc(sizeof(*q) * n);
    q[0] = destination;
    o = 1;

    totalDis = 0;
    middleNode = destination;
    do
    {
        parentNode = t[middleNode];

        if (parentNode < n)
        {
            totalDis += m[indexOf(parentNode, middleNode, n)];
            q[o] = parentNode;
            o += 1;

            middleNode = parentNode;
        }
        else
        {
            o = 0;
            break;
        }
    } while (parentNode != source);

    for (i = 0; i < o; i += 1)
        p[i] = q[o - i - 1];
    free(q);

    if (out_dis)
        *out_dis = totalDis;

    return o;
}

int main(void)
{
    int *m;
    unsigned int *t, *path;
    unsigned int n, i, d, j, k, o, p, s, dis, numHops;

    printf("Number of test cases: ");
    scanf("%u", &n);

    for (i = 0; i < n; i += 1)
    {
        printf("Number of nodes: ");
        scanf("%u", &d);
        k = (d * (d - 1)) >> 1;
        m = (int *)malloc(sizeof(*m) * (k + 2));
        t = (unsigned int *)malloc(sizeof(*t) * d);
        path = (unsigned int *)malloc(sizeof(*path) * d);
        printf("Matrix data (distance < 0 means no connection): \n");
        for (j = 1; j < d; j += 1)
        {
            printf("Distances from Node #%u to Node #%u", j, j + 1);
            o = d - j;
            if (o > 1)
                printf(" ~ #%u", d);
            printf(" (1 x %u Matrix): ", o);
            for (p = 0; p < o; p += 1)
            {
                scanf("%d", m + indexOf(j - 1, j + p, d));
                printf("[DEBUG] Node #%u to #%u: %u at (%u)\n", j, j + p + 1, (unsigned int)m[indexOf(j - 1, j + p, d)], indexOf(j - 1, j + p, d));
            }
        }
        m[k] = 0;
        m[k + 1] = -1;
        printf("Matrix Table: \n");
        printMatrix(m, d);
        printf("\n");
        printf("Start Point: (1 ~ %u): ", d);
        scanf("%u", &s);
        s -= 1;
        dijkstra(m, t, d, s);
        for (j = 0; j < d; j += 1)
        {
            if (j == s)
                continue;

            numHops = dijkstraPath(m, t, path, &dis, d, s, j);
            printf("Distance from Node #%u to Node #%u: %u\n", s + 1, j + 1, dis);
            printf("Path: #%u", path[0] + 1);
            for (p = 1; p < numHops; p += 1)
            {
                printf("-#%u", path[p] + 1);
            }
            printf("\n\n");
        }

        free(m);
        free(t);
        free(path);
    }

    return 0;
}
