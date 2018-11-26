#include "set.h"
#include <assert.h>
#include <stdlib.h>
#include "da.h"

struct set {
  DA *nodes;
  void (*display)(void *, FILE *);
  void (*release)(void *);
};

typedef struct node {
  void *value;
  int index;
  int rank;
  struct node *parent;
  void (*display)(void *, FILE *);
  void (*release)(void *);
} NODE;

NODE *newNODE(SET *s, void *value, int index) {
  NODE *n = malloc(sizeof(NODE));
  assert(n != 0);
  n->value = value;
  n->index = index;
  n->rank = 0;
  n->parent = n;
  n->display = s->display;
  n->release = s->release;
  return n;
}

void displayNODE(void *v, FILE *fp) {
  NODE *n = v;
  if (n->display) n->display(n->value, fp);
  // fprintf(fp, ", index: %d", n->index);
}

void freeNODE(void *v) {
  NODE *n = v;
  if (n->release) n->release(n->value);
  free(n);
}

SET *newSET(void) {
  SET *s = malloc(sizeof(SET));
  assert(s != 0);
  s->nodes = newDA();
  s->display = 0;
  s->release = 0;
  setDAdisplay(s->nodes, displayNODE);
  setDAfree(s->nodes, freeNODE);
  return s;
}

void setSETdisplay(SET *s, void (*d)(void *, FILE *)) { s->display = d; }

void setSETfree(SET *s, void (*f)(void *)) { s->release = f; }

int makeSET(SET *s, void *value) {
  int index = sizeDA(s->nodes);
  insertDAback(s->nodes, newNODE(s, value, index));
  return index;
}

int findSET(SET *s, int index) {
  NODE *n = getDA(s->nodes, index);
  if (n->parent != n) n->parent = getDA(s->nodes, findSET(s, n->parent->index));
  return n->parent->index;
}

int unionSET(SET *s, int index1, int index2) {
  NODE *rep1 = getDA(s->nodes, findSET(s, index1));
  NODE *rep2 = getDA(s->nodes, findSET(s, index2));
  if (rep1 != rep2) {
    if (rep2->rank > rep1->rank || (rep2->rank == rep1->rank && rep2->index < rep1->index)) {
      NODE *tempr = rep1;
      rep1 = rep2;
      rep2 = tempr;
    }
    rep2->parent = rep1;
    if (rep2->rank == rep1->rank) rep1->rank++;
  }
  return rep1->index;
}

void displaySET(SET *s, FILE *fp) {
  for (int i = 0, len = sizeDA(s->nodes); i < len; i++) {
    fprintf(fp, "%d:", i);
    NODE *n = getDA(s->nodes, i);
    while (1) {
      fprintf(fp, " ");
      displayNODE(n, fp);
      if (getDA(s->nodes, i) == n->parent) fprintf(fp, "(%d)", n->rank);
      if (n == n->parent) break;
      n = n->parent;
    }
    fprintf(fp, "\n");
  }
}

int debugSET(SET *s, int level) { 
  s = s;
  level = level;
  return 0;
}

void freeSET(SET *s) {
  freeDA(s->nodes);
  free(s);
}