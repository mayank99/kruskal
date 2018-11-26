#include "edge.h"
#include <assert.h>
#include <stdlib.h>

EDGE *newEDGE(int v1, int v2, int weight) {
  EDGE *edge = malloc(sizeof(EDGE));
  assert(edge != 0);
  edge->v1 = v1;
  edge->v2 = v2;
  edge->weight = weight;
  return edge;
}

int compareEDGEweight(const void *a, const void *b) {
  EDGE *x = *(EDGE **)a;
  EDGE *y = *(EDGE **)b;
  int diff = x->weight - y->weight;
  if (diff == 0) diff = x->v2 - y->v2;
  if (diff == 0) diff = x->v1 - y->v1;
  return diff;
}

int compareEDGE(void *a, void *b) {
  int diff = ((EDGE *)a)->v1 - ((EDGE *)b)->v1;
  if (diff == 0) diff = ((EDGE *)a)->v2 - ((EDGE *)b)->v2;
  return diff;
}

void displayEDGE(void *edge, FILE *fp) {
  EDGE *e = edge;
  fprintf(fp, "%d->%d (%d)", e->v1, e->v2, e->weight);
}

// void freeEDGE(void *edge) { free(edge); }