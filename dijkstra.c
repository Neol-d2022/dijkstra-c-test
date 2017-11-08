#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

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

typedef struct
{
    unsigned int nodeLinkSource;
    unsigned int nodeLinkDestination;
    unsigned int linkCost;
    unsigned int totalCost;
    unsigned int totalNumberOfHops;
} dijkstra_node_t;

int cmpUInt(const void *a, const void *b)
{
    unsigned int c = *(unsigned int *)a;
    unsigned int d = *(unsigned int *)b;

    if (c > d)
        return 1;
    else if (c < d)
        return -1;
    else
        return 0;
}

int cmpDijkstraNode(const void *a, const void *b)
{
    dijkstra_node_t *c = (dijkstra_node_t *)a;
    dijkstra_node_t *d = (dijkstra_node_t *)b;
    int r;

    if ((r = cmpUInt(&(c->totalCost), &(d->totalCost))) != 0)
        return r;
    else if ((r = cmpUInt(&(c->totalNumberOfHops), &(d->totalNumberOfHops))) != 0)
        return r;
    else if ((r = cmpUInt(&(c->linkCost), &(d->linkCost))) != 0)
        return r;
    else if ((r = cmpUInt(&(c->nodeLinkSource), &(d->nodeLinkSource))) != 0)
        return r;
    else if ((r = cmpUInt(&(c->nodeLinkSource), &(d->nodeLinkDestination))) != 0)
        return r;
    else
        return 0;
}

unsigned int dijkstra(int *m, unsigned int *t, unsigned int n, unsigned int s)
{
    Queue_t pq;
    dijkstra_node_t *dn;
    unsigned char *nodeKnown;
    unsigned int i, j, d, numKnownNodes, numHops, lastKnownNode, lastNumHops, lastDis;
    int dis;

    QueueInit(&pq);
    QueueSetComparer(&pq, cmpDijkstraNode);
    nodeKnown = (unsigned char *)malloc(sizeof(*nodeKnown) * n);
    memset(nodeKnown, 0, sizeof(*nodeKnown) * n);
    nodeKnown[s] = 1;
    numKnownNodes = 1;
    t[s] = n;
    lastKnownNode = s;
    lastNumHops = 0;
    lastDis = 0;

    do
    {
        i = lastKnownNode;
        for (j = 0; j < n; j += 1)
        {
            if (nodeKnown[j] == 1)
                continue;
            if ((dis = m[indexOf(i, j, n)]) >= 0)
            {
                d = dis + lastDis;
                numHops = lastNumHops + 1;

                dn = (dijkstra_node_t *)malloc(sizeof(*dn));
                dn->nodeLinkSource = lastKnownNode;
                dn->nodeLinkDestination = j;
                dn->linkCost = (unsigned int)dis;
                dn->totalCost = d;
                dn->totalNumberOfHops = numHops;
                printf("#%u-#%u: T:%u, H:%u, L:%u (enqueued)\n", dn->nodeLinkSource + 1, dn->nodeLinkDestination + 1, dn->totalCost, dn->totalNumberOfHops, dn->linkCost);

                QueueEnqueue(&pq, dn);
            }
        }

        while (!QueueEmpty(&pq))
        {
            dn = (dijkstra_node_t *)QueueDequeue(&pq);

            if (nodeKnown[dn->nodeLinkDestination])
            {
                printf("#%u-#%u: T:%u, H:%u, L:%u (already learned)\n", dn->nodeLinkSource + 1, dn->nodeLinkDestination + 1, dn->totalCost, dn->totalNumberOfHops, dn->linkCost);
                free(dn);
            }
            else
            {
                nodeKnown[dn->nodeLinkDestination] = 1;
                lastKnownNode = dn->nodeLinkDestination;
                lastNumHops = dn->totalNumberOfHops;
                lastDis = dn->totalCost;
                t[dn->nodeLinkDestination] = dn->nodeLinkSource;
                numKnownNodes += 1;

                printf("#%u-#%u: T:%u, H:%u, L:%u (dequeued)\n", dn->nodeLinkSource + 1, dn->nodeLinkDestination + 1, dn->totalCost, dn->totalNumberOfHops, dn->linkCost);
                free(dn);
                break;
            }
        }

        if (numKnownNodes == n)
            break;
    } while (!QueueEmpty(&pq));

    free(nodeKnown);

    while (!QueueEmpty(&pq))
    {
        dn = (dijkstra_node_t *)QueueDequeue(&pq);
        printf("#%u-#%u: T:%u, H:%u, L:%u (leftover)\n", dn->nodeLinkSource + 1, dn->nodeLinkDestination + 1, dn->totalCost, dn->totalNumberOfHops, dn->linkCost);
        free(dn);
    }
    QueueDeInit(&pq);

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
                printf("[DEBUG] Node #%u to #%u: %d at (%u)\n", j, j + p + 1, m[indexOf(j - 1, j + p, d)], indexOf(j - 1, j + p, d));
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
            printf("Shortest distance from Node #%u to Node #%u: %u\n", s + 1, j + 1, dis);
            printf("Shortest Path from Node #%u to Node #%u:\n", s + 1, path[numHops - 1] + 1);
            printf("#%u", path[0] + 1);
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
