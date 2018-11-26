#include "tnode.h"
#include <assert.h>
#include <stdlib.h>

struct tnode {
  void *value;
  TNODE *left;
  TNODE *right;
  TNODE *parent;
  int debug;
  void (*display)(void *value, FILE *fp);
  void (*release)(void *value);
};

TNODE *newTNODE(void *v, TNODE *l, TNODE *r, TNODE *p) {
  TNODE *n = malloc(sizeof(TNODE));
  assert(n != 0);
  n->value = v;
  n->left = l;
  n->right = r;
  n->parent = p;
  n->debug = 0;
  n->display = 0;
  n->release = 0;
  return n;
}

void setTNODEdisplay(TNODE *n, void (*d)(void *value, FILE *fp)) { n->display = d; }

void setTNODEfree(TNODE *n, void (*f)(void *value)) { n->release = f; }

void *getTNODEvalue(TNODE *n) { return n->value; }

void setTNODEvalue(TNODE *n, void *replacement) { n->value = replacement; }

TNODE *getTNODEleft(TNODE *n) { return n->left; }

void setTNODEleft(TNODE *n, TNODE *replacement) { n->left = replacement; }

TNODE *getTNODEright(TNODE *n) { return n->right; }

void setTNODEright(TNODE *n, TNODE *replacement) { n->right = replacement; }

TNODE *getTNODEparent(TNODE *n) { return n->parent; }

void setTNODEparent(TNODE *n, TNODE *replacement) { n->parent = replacement; }

void displayTNODE(TNODE *n, FILE *fp) {
  if (n->display != 0) {
    n->display(n->value, fp);
  } else {
    fprintf(fp, "&%p", n->value);
  }
  if (n->debug > 0) {
    fprintf(fp, "&%p", n);
  }
}

int debugTNODE(TNODE *n, int level) {
  int tmp = n->debug;
  n->debug = level;
  return tmp;
}

void freeTNODE(TNODE *n) {
  if (n->release != 0) {
    n->release(n->value);
  }
  free(n);
}