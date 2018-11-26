#include "bst.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

struct bst {
  TNODE *root;
  int size;
  int debug;
  int (*compare)(void *, void *);
  void (*display)(void *, FILE *);
  void (*swap)(TNODE *, TNODE *);
  void (*release)(void *);
};

static void swap(TNODE *a, TNODE *b) {
  void *tmp = getTNODEvalue(a);
  setTNODEvalue(a, getTNODEvalue(b));
  setTNODEvalue(b, tmp);
}

BST *newBST(int (*c)(void *, void *)) {
  BST *t = malloc(sizeof(BST));
  assert(t != 0);
  t->compare = c;
  t->size = 0;
  t->root = 0;
  t->debug = 0;
  t->display = 0;
  t->swap = swap;
  t->release = 0;
  return t;
}

void setBSTdisplay(BST *t, void (*d)(void *, FILE *)) { t->display = d; }

void setBSTswapper(BST *t, void (*s)(TNODE *, TNODE *)) { t->swap = s; }

void setBSTfree(BST *t, void (*f)(void *)) { t->release = f; }

TNODE *getBSTroot(BST *t) { return t->root; }

void setBSTroot(BST *t, TNODE *replacement) {
  t->root = replacement;
  setTNODEparent(t->root, t->root);
}

void setBSTsize(BST *t, int s) { t->size = s; }

static void insertBSTinternal(BST *t, TNODE *new) {  
  TNODE *node = t->root;
  do {
    TNODE *child = 0;
    if (t->compare(getTNODEvalue(new), getTNODEvalue(node)) < 0) {
      child = getTNODEleft(node);
      if (child == 0) setTNODEleft(node, new);
    } else if (t->compare(getTNODEvalue(new), getTNODEvalue(node)) > 0) {
      child = getTNODEright(node);
      if (child == 0) setTNODEright(node, new);
    }
    if (child == 0) setTNODEparent(new, node);
    node = child;
  } while (node != 0);
}

TNODE *insertBST(BST *t, void *value) {
  TNODE *node = newTNODE(value, 0, 0, 0);
  assert(node != 0);
  // setTNODEparent(node, node);
  if (t->size == 0) setBSTroot(t, node);
  else insertBSTinternal(t, node);
  t->size++;
  return node;
}

static void *findBSTinternal(TNODE *node, void *key, int (*compare)(void *, void *)) {
  if (node == 0 || compare(getTNODEvalue(node), key) == 0) {
    return node;
  } else if (compare(getTNODEvalue(node), key) > 0) {
    return findBSTinternal(getTNODEleft(node), key, compare);
  } else {
    return findBSTinternal(getTNODEright(node), key, compare);
  }
}

void *findBST(BST *t, void *key) {
  TNODE *node = findBSTinternal(t->root, key, t->compare);
  return node == 0 ? 0 : getTNODEvalue(node);
}

TNODE *locateBST(BST *t, void *key) { return findBSTinternal(t->root, key, t->compare); }

int deleteBST(BST *t, void *key) {
  TNODE *node = locateBST(t, key);
  if (node == 0) return -1;
  TNODE *leaf = swapToLeafBST(t, node);
  pruneLeafBST(t, leaf);
  free(leaf);
  t->size--;
  if (t->size == 0) t->root = 0;
  return 0;
}

static TNODE *successorBST(TNODE *node) {
  TNODE *tmp = node;
  TNODE *right = getTNODEright(node);
  if (right != 0) {
    tmp = right;
    while (getTNODEleft(tmp) != 0) tmp = getTNODEleft(tmp);
  }
  return tmp;
}

static TNODE *predecessorBST(TNODE *node) {
  TNODE *tmp = node;
  TNODE *left = getTNODEleft(node);
  if (left != 0) {
    tmp = left;
    while (getTNODEright(tmp) != 0) tmp = getTNODEright(tmp);
  }
  return tmp;
}

static int isBSTleaf(TNODE *n) { return getTNODEleft(n) || getTNODEright(n) ? 0 : 1; }

TNODE *swapToLeafBST(BST *t, TNODE *node) {
  TNODE *leaf = node;
  TNODE *(*sucessorOrPredecessor)(TNODE *) = getTNODEleft(leaf) != 0 ? predecessorBST : successorBST;
  while (!isBSTleaf(leaf)) {
    TNODE *next = sucessorOrPredecessor(leaf);
    if (next != 0) t->swap(leaf, next);
    leaf = next;
  }
  return leaf;
}

void pruneLeafBST(BST *t, TNODE *leaf) {
  TNODE *parent = getTNODEparent(leaf);
  if (parent != 0 && parent != leaf) {
    if (getTNODEleft(parent) == leaf) setTNODEleft(parent, 0);
    else if (getTNODEright(parent) == leaf) setTNODEright(parent, 0);
  } else if (parent == leaf) {
    t->root = 0;
  }
}

int sizeBST(BST *t) { return t->size; }

static void recur(TNODE *n, int level, int *min, int *max) {
    if (n != 0) {
      level++;
      if ((!getTNODEleft(n) || !getTNODEright(n)) && (*min == -1 || level < *min)) *min = level;
      if (isBSTleaf(n) && level > *max) *max = level;
      recur(getTNODEleft(n), level, min, max);
      recur(getTNODEright(n), level, min, max);
    }
}

void statisticsBST(BST *t, FILE *fp) {
  int min = -1, max = -1;
  recur(t->root, -1, &min, &max);
  fprintf(fp, "Nodes: %d\n", t->size);
  fprintf(fp, "Minimum depth: %d\n", min);
  fprintf(fp, "Maximum depth: %d\n", max);
}

// static void traverseBST(TNODE *root) {
//   QUEUE *queue = newQUEUE();
//   enqueue(queue, root);
//   while (sizeQUEUE(queue) > 0) {
//     TNODE *node = dequeue(queue);
//     enqueue(queue, getTNODEleft(node));
//     enqueue(queue, getTNODEright(node));
//   }
//   freeQUEUE(queue);
// }

typedef struct qnode { // used for displayBSTlevel
  TNODE *tnode;
  int level;
  char side;
} QNODE;

static QNODE *newQNODE(TNODE *n, int level, char side) {
  QNODE *node = malloc(sizeof(QNODE));
  assert(node != 0);
  node->tnode = n;
  node->level = level;
  node->side = side;
  return node;
}

static void displayBSTlevel(BST *t, FILE *fp) {
  QUEUE *queue = newQUEUE();
  if (t->size == 0) fprintf(fp, "0:");
  else enqueue(queue, newQNODE(t->root, 0, 'X'));
  int level = -1;
  while (sizeQUEUE(queue) > 0) {
    QNODE *node = dequeue(queue);
    if (node->level == level + 1) {
      level++;
      if (level != 0) fprintf(fp, "\n");
      fprintf(fp, "%d:", level);
    }

    fprintf(fp, " ");
    if (isBSTleaf(node->tnode)) fprintf(fp, "=");
    t->display(getTNODEvalue(node->tnode), fp);
    fprintf(fp, "(");
    TNODE *parent = getTNODEparent(node->tnode);
    if (parent) t->display(getTNODEvalue(parent), fp);
    fprintf(fp, ")%c", node->side);

    if (getTNODEleft(node->tnode) != 0) enqueue(queue, newQNODE(getTNODEleft(node->tnode), node->level + 1, 'L'));
    if (getTNODEright(node->tnode) != 0) enqueue(queue, newQNODE(getTNODEright(node->tnode), node->level + 1, 'R'));
    free(node);
  }
  fprintf(fp, "\n");
  freeQUEUE(queue);
}

static void displayBSTdebug(BST *t, TNODE *n, FILE *fp) {
    if (n != 0) {
        TNODE *l = getTNODEleft(n), *r = getTNODEright(n);
        
        if (t->debug == 2) t->display(getTNODEvalue(n), fp);

        if (l != 0) {
            if (t->debug == 2) fprintf(fp, " ");
            fprintf(fp, "[");
            displayBSTdebug(t, l, fp);
            fprintf(fp, "]");
            if (t->debug == 1 || t->debug == 3) fprintf(fp, " ");
        }

        if (t->debug == 1) t->display(getTNODEvalue(n), fp);

        if (r != 0) {
            if (t->debug == 1 || t->debug == 2) fprintf(fp, " ");
            fprintf(fp, "[");
            displayBSTdebug(t, r, fp);
            fprintf(fp, "]");
            if (t->debug > 2) fprintf(fp, " ");
        }

        if (t->debug == 3) t->display(getTNODEvalue(n), fp);
    }
}

void displayBST(BST *t, FILE *fp) {
  if (t->debug == 0) displayBSTlevel(t, fp);
  else {
    fprintf(fp, "[");
    displayBSTdebug(t, t->root, fp);
    fprintf(fp, "]");
  }
}

int debugBST(BST *t, int level) {
  int tmp = t->debug;
  t->debug = level;
  return tmp;
}

static void freeBSTrecur(TNODE *n, void(*release)(void *)) {
  if (n != 0) {
    if (getTNODEleft(n) != 0) freeBSTrecur(getTNODEleft(n), release);
    if (getTNODEright(n) != 0) freeBSTrecur(getTNODEright(n), release);
    if (release != 0) release(getTNODEvalue(n));
    freeTNODE(n);
  }
}

void freeBST(BST *t) {
  freeBSTrecur(t->root, t->release);
  free(t);
}