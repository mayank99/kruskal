
#include "gst.h"
#include <assert.h>
#include <stdlib.h>
#include "bst.h"

struct gst {
  // TNODE *root;
  BST *bst;
  int size;
  int insertions;
  // int debug;
  int (*compare)(void *, void *);
  void (*display)(void *, FILE *);
  void (*swap)(TNODE *, TNODE *);
  void (*release)(void *);
};

typedef struct gstval {
  void *value;
  int frequency;
  void (*display)(void *value, FILE *fp);
  int (*compare)(void *a, void *b);
  void (*release)(void *value);
} GSTVAL;

GSTVAL *newGSTVAL(GST *t, void *v) {
  GSTVAL *n = malloc(sizeof(GSTVAL));
  assert(n != 0);
  n->value = v;
  n->frequency = 1;
  n->display = t->display;
  n->compare = t->compare;
  n->release = t->release;
  return n;
}

static void swapGSTVAL(TNODE *a, TNODE *b) {
  void *x = getTNODEvalue(a);
  void *y = getTNODEvalue(b);
  setTNODEvalue(a, y);
  setTNODEvalue(b, x);
}

static int compareGSTVAL(void *a, void *b) {
  GSTVAL *x = a;
  GSTVAL *y = b;
  return x->compare(x->value, y->value);
}

static void displayGSTVAL(void *n, FILE *fp) {
  GSTVAL *gn = (GSTVAL *)n;
  gn->display(gn->value, fp);
  if (gn->frequency > 1) fprintf(fp, "<%d>", gn->frequency);
}

static void freeGSTVAL(void *n) {
  GSTVAL *gn = n;
  if (gn->release) gn->release(gn->value);
  free(gn);
}

GST *newGST(int (*c)(void *, void *)) {
  GST *t = malloc(sizeof(GST));
  assert(t != 0);
  t->compare = c;
  t->size = 0;
  t->insertions = 0;
  // t->root = 0;
  // t->debug = 0;
  t->display = 0;
  t->swap = swapGSTVAL;
  t->release = 0;
  t->bst = newBST(compareGSTVAL);
  setBSTdisplay(t->bst, displayGSTVAL);
  setBSTfree(t->bst, freeGSTVAL);
  return t;
}

void setGSTdisplay(GST *t, void (*d)(void *, FILE *)) { t->display = d; }

void setGSTswapper(GST *t, void (*s)(TNODE *, TNODE *)) {
  t->swap = s;
  setBSTswapper(t->bst, s);
}

void setGSTfree(GST *t, void (*f)(void *)) { t->release = f; }

TNODE *getGSTroot(GST *t) { return getBSTroot(t->bst); }

void setGSTroot(GST *t, TNODE *replacement) {
  setBSTroot(t->bst, replacement);
  setTNODEparent(replacement, replacement);
}

void setGSTsize(GST *t, int s) { t->size = s; setBSTsize(t->bst, s); }

TNODE *insertGST(GST *t, void *value) {
  TNODE *n = locateGST(t, value);
  t->size++;
  if (n) {
    GSTVAL *gstval = getTNODEvalue(n);
    gstval->frequency++;
    if (t->release) t->release(value);
    t->insertions++;
    return 0;
  } else {
    GSTVAL *val = newGSTVAL(t, value);
    return insertBST(t->bst, val);
  }
}

void *findGST(GST *t, void *key) {
  GSTVAL *gstval = newGSTVAL(t, key);
  GSTVAL *value = findBST(t->bst, gstval);
  free(gstval);
  return value ? value->value : 0;
}

TNODE *locateGST(GST *t, void *key) {
  GSTVAL *gstval = newGSTVAL(t, key);
  TNODE *node = locateBST(t->bst, gstval);
  free(gstval);
  return node;
}

int deleteGST(GST *t, void *key) {
  TNODE *n = locateGST(t, key);
  int code = -1;
  if (n) {
    GSTVAL *value = getTNODEvalue(n);
    value->frequency--;
    code = value->frequency;
    if (value->frequency == 0) {
      deleteBST(t->bst, value);
      free(value);
    }
    else t->insertions--;
    t->size--;
  }
  return code;
}

TNODE *swapToLeafGST(GST *t, TNODE *node) { return swapToLeafBST(t->bst, node); }

void pruneLeafGST(GST *t, TNODE *leaf) { 
  GSTVAL *gstval = getTNODEvalue(leaf);
  free(gstval);
  pruneLeafBST(t->bst, leaf);
}

int sizeGST(GST *t) { return sizeBST(t->bst); }

void statisticsGST(GST *t, FILE *fp) {
  fprintf(fp, "Duplicates: %d\n", t->insertions);
  statisticsBST(t->bst, fp);
}

void displayGST(GST *t, FILE *fp) { displayBST(t->bst, fp); }

int debugGST(GST *t, int level) { return debugBST(t->bst, level); }

void freeGST(GST *t) {
  freeBST(t->bst);
  free(t);
}

void *unwrapGST(TNODE *n) {
  GSTVAL *value = getTNODEvalue(n);
  return value ? value->value : 0;
}

int freqGST(GST *t, void *key) {
  TNODE *node = locateGST(t, key);
  GSTVAL *gstval = node ? getTNODEvalue(node) : 0;
  return gstval ? gstval->frequency : 0;
}

int duplicatesGST(GST *g) { return g->size - sizeBST(g->bst); }