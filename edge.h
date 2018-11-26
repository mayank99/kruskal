#ifndef __EDGE_INCLUDED__
#define __EDGE_INCLUDED__

#include <stdio.h>

typedef struct edge EDGE;

struct edge {
  int v1;
  int v2;
  int weight;
};

extern EDGE *newEDGE(int v1, int v2, int weight);
extern int compareEDGE(void *, void *);
extern int compareEDGEweight(const void *a, const void *b);
extern void displayEDGE(void *, FILE *);
// extern void freeEDGE(void *);
#endif