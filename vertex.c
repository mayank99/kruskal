#include "vertex.h"
#include <assert.h>
#include <stdlib.h>

VERTEX *newVERTEX(int v) {
  VERTEX *vertex = malloc(sizeof(VERTEX));
  assert(vertex != 0);
  vertex->v = v;
  vertex->flag = -1;
  vertex->key = 0;
  vertex->predecessor = 0;
  vertex->level = -1;
  vertex->neighbors = newDA();
  return vertex;
}

 NEIGHBOR *newNEIGHBOR(VERTEX *v, int w, int active) {
   NEIGHBOR *n = malloc(sizeof(NEIGHBOR));
   assert(n != 0);
   n->vertex = v;
   n->weight = w;
   n->active = active;
   return n;
 }

int indexVERTEXneighbor(VERTEX *a, VERTEX *b) {
  for (int i = 0; i < sizeDA(a->neighbors); i++) {
    NEIGHBOR *n = getDA(a->neighbors, i);
    if (n->vertex == b) return i;
  }
  return -1;
}

void displayVERTEX(void *vertex, FILE *fp) {
  VERTEX *v = vertex;
  fprintf(fp, "%d", v->v);
  if (v->predecessor != 0) fprintf(fp, "(%d)%d", v->predecessor->v, v->flag);
}

int compareVERTEX(const void *a, const void *b) {
  VERTEX *x = *(VERTEX **)a;
  VERTEX *y = *(VERTEX **)b;
  int diff = x->level - y->level;
  if (diff == 0) diff = x->flag - y->flag;
  if (diff == 0) diff = x->predecessor->v - y->predecessor->v;
  if (diff == 0) diff = x->v - y->v;
  return diff;
}

void setNEIGHBORactive(NEIGHBOR *n, int active) {
  n->active = active;
}

int compareNEIGHBOR(const void *a, const void *b) {
  NEIGHBOR *x = *(NEIGHBOR **)a;
  NEIGHBOR *y = *(NEIGHBOR **)b;
  int diff = x->weight - y->weight;
  if (diff == 0) diff = x->vertex->v - y->vertex->v;
  return diff;
}

void freeVERTEX(void *v) {
  VERTEX *vertex = v;
  setDAfree(vertex->neighbors, free);
  freeDA(vertex->neighbors);
  free(vertex);
}