
#include "rbt.h"
#include <assert.h>
#include <stdlib.h>
#include "gst.h"

#define RED 1
#define BLACK 0

struct rbt {
	// TNODE *root;
	GST *gst;
	int size;
	int insertions;
	// int debug;
	int (*compare)(void *, void *);
	void (*display)(void *, FILE *);
	void (*swap)(TNODE *, TNODE *);
	void (*release)(void *);
};

typedef struct rbtval {
	void *value;
	int color;
	RBT *rbt;
} RBTVAL;

RBTVAL *newRBTVAL(RBT *t, void *v) {
	RBTVAL *n = malloc(sizeof(RBTVAL));
	assert(n != 0);
	n->value = v;
	n->color = 0;
	n->rbt = t;
	return n;
}

static void swapRBTVAL(TNODE *a, TNODE *b) {
	void *va = getTNODEvalue(a);
	void *vb = getTNODEvalue(b);
	setTNODEvalue(a, vb);
	setTNODEvalue(b, va);

	// swap the colors back to the original nodes
	RBTVAL *x = unwrapGST(a);
	RBTVAL *y = unwrapGST(b);
	int color = x->color;
	x->color = y->color;
	y->color = color;
}

static void setTNODEcolor(TNODE *n, int c) {
	RBTVAL *v = n ? unwrapGST(n) : 0;
	if (v) v->color = c;
}

static int getTNODEcolor(TNODE *n) { 
	RBTVAL *v = n ? unwrapGST(n) : 0;
	return v ? v->color : 0;
}

static int compareRBTVAL(void *a, void *b) {
	RBTVAL *x = a;
	RBTVAL *y = b;
	return x->rbt->compare(x->value, y->value);
}

static void displayRBTVAL(void *n, FILE *fp) {
	RBTVAL *rn = (RBTVAL *)n;
	rn->rbt->display(rn->value, fp);
	if (rn->color == 1) fprintf(fp, "*");
}

static void freeRBTVAL(void *n) {
  RBTVAL *rn = n;
  if (rn->rbt->release) rn->rbt->release(rn->value);
  free(rn);
}

RBT *newRBT(int (*c)(void *, void *)) {
	RBT *t = malloc(sizeof(RBT));
	assert(t != 0);
	t->compare = c;
	t->size = 0;
	t->insertions = 0;
	// t->root = 0;
	// t->debug = 0;
	t->display = 0;
	t->swap = swapRBTVAL;
	t->release = 0;
	t->gst = newGST(compareRBTVAL);
	setGSTdisplay(t->gst, displayRBTVAL);
	setGSTswapper(t->gst, t->swap);
  setGSTfree(t->gst, freeRBTVAL);
	return t;
}

void setRBTdisplay(RBT *t, void (*d)(void *, FILE *)) { t->display = d; }

void setRBTswapper(RBT *t, void (*s)(TNODE *, TNODE *)) { t->swap = s; }

void setRBTfree(RBT *t, void (*f)(void *)) { t->release = f; }

TNODE *getRBTroot(RBT *t) { return getGSTroot(t->gst); }

void setRBTroot(RBT *t, TNODE *replacement) {
	setGSTroot(t->gst, replacement);
	setTNODEparent(replacement, replacement);
}

void setRBTsize(RBT *t, int s) {
	t->size = s;
	setGSTsize(t->gst, s);
}

static TNODE *getTNODEuncle(TNODE *n) {
	TNODE *parent = getTNODEparent(n);
	TNODE *grandparent = getTNODEparent(parent);
	if (parent == getTNODEleft(grandparent)) return getTNODEright(grandparent);
	else return getTNODEleft(grandparent);
}

static void rotateRBTright(RBT *t, TNODE *x) {
	TNODE *y = getTNODEleft(x);
	TNODE *parent = getTNODEparent(x);

	setTNODEleft(x, getTNODEright(y));
	if (getTNODEright(y)) setTNODEparent(getTNODEright(y), x);
	setTNODEparent(y, parent);

	if (parent == x) setGSTroot(t->gst, y);
	else if (x == getTNODEleft(parent)) setTNODEleft(parent, y);
	else setTNODEright(parent, y);

	setTNODEright(y, x);
	setTNODEparent(x, y);
}

static void rotateRBTleft(RBT *t, TNODE *x) {
	TNODE *y = getTNODEright(x);
	TNODE *parent = getTNODEparent(x);

	setTNODEright(x, getTNODEleft(y));
	if (getTNODEleft(y)) setTNODEparent(getTNODEleft(y), x);
	setTNODEparent(y, parent);

	if (parent == x) setGSTroot(t->gst, y);
	else if (x == getTNODEleft(parent)) setTNODEleft(parent, y);
	else setTNODEright(parent, y);

	setTNODEleft(y, x);
	setTNODEparent(x, y);
}

static void rotateRBT(TNODE *a, TNODE *b) {
	RBTVAL *v = unwrapGST(a);
	if (a == getTNODEleft(b)) {
		rotateRBTright(v->rbt, b);
	} else {
		rotateRBTleft(v->rbt, b);
	}
}

static void insertRBTfixup(RBT *t, TNODE *x) {
	while (1) {
		TNODE *parent = getTNODEparent(x);

		if (x == getRBTroot(t)) break;
		if (getTNODEcolor(parent) == BLACK) break;

		TNODE *grandparent = getTNODEparent(getTNODEparent(x));
		TNODE *uncle = getTNODEuncle(x);
		
		if (getTNODEcolor(uncle) == RED) {
			setTNODEcolor(parent, BLACK);
			setTNODEcolor(uncle, BLACK);
			setTNODEcolor(grandparent, RED);
			x = grandparent;
		} else {
			if (!(x == getTNODEleft(parent) && parent == getTNODEleft(grandparent)) &&
				!(x == getTNODEright(parent) && parent == getTNODEright(grandparent))) {
					rotateRBT(x, parent);
					TNODE *tmp = x;
					x = parent;
					parent = tmp;
			}
			setTNODEcolor(parent, BLACK);
			setTNODEcolor(grandparent, RED);
			rotateRBT(parent, grandparent);
			break;
		}
	}
}

TNODE *insertRBT(RBT *t, void *value) {
	TNODE *n = insertGST(t->gst, newRBTVAL(t, value));
	if (n) {
		setTNODEcolor(n, RED);
		insertRBTfixup(t, n);
		setTNODEcolor(getRBTroot(t), BLACK);
	}
	return n;
}

void *findRBT(RBT *t, void *key) {
	RBTVAL *rbtval = newRBTVAL(t, key);
	RBTVAL *value = findGST(t->gst, rbtval);
	free(rbtval);
	return value ? value->value : 0;
}

TNODE *locateRBT(RBT *t, void *key) {
	RBTVAL *rbtval = newRBTVAL(t, key);
	TNODE *node = locateGST(t->gst, rbtval);
	free(rbtval);
	return node;
}

static void deleteRBTfixup(RBT *t, TNODE *x) {
	while (x != getRBTroot(t) && getTNODEcolor(x) == BLACK) {
		TNODE *parent = getTNODEparent(x);
		TNODE *sibling = x == getTNODEleft(parent) ? getTNODEright(parent) : getTNODEleft(parent);
		
		TNODE *niece = 0;
		TNODE *nephew = 0;
		if (x == getTNODEleft(parent) && sibling != 0) {
			niece = getTNODEleft(sibling);
			nephew = getTNODEright(sibling);
		} else if (x == getTNODEright(parent) && sibling != 0) {
			nephew = getTNODEleft(sibling);
			niece = getTNODEright(sibling);
		}

		if (getTNODEcolor(sibling) == RED) {
			setTNODEcolor(parent, RED);
			setTNODEcolor(sibling, BLACK);
			rotateRBT(sibling, parent);
		} else if (getTNODEcolor(nephew) == RED)  {
			setTNODEcolor(sibling, getTNODEcolor(parent));
			setTNODEcolor(parent, BLACK);
			setTNODEcolor(nephew, BLACK);
			rotateRBT(sibling, parent);
			break;
		} else if (getTNODEcolor(niece) == RED) {
			setTNODEcolor(niece, BLACK);
			setTNODEcolor(sibling, RED);
			rotateRBT(niece, sibling);
		} else {
			setTNODEcolor(sibling, RED);
			x = parent;
		}
	}
	setTNODEcolor(x, BLACK);
}

int deleteRBT(RBT *t, void *key) {
	int code = -1;
	if (freqRBT(t, key) > 1) {
		RBTVAL *rbtval = newRBTVAL(t, key);
		code = deleteGST(t->gst, rbtval);
		free(rbtval);
	} else {
		TNODE *n = locateRBT(t, key);
		if (n) {
			TNODE *leaf = swapToLeafRBT(t, n);
			RBTVAL *rbtval = unwrapGST(leaf);
			deleteRBTfixup(t, leaf);
			pruneLeafRBT(t, leaf);
			free(rbtval);
			free(leaf);
			code = 0;
			setGSTsize(t->gst, sizeGST(t->gst) - 1);
		}
	}
	return code;
}

TNODE *swapToLeafRBT(RBT *t, TNODE *node) { return swapToLeafGST(t->gst, node); }

void pruneLeafRBT(RBT *t, TNODE *leaf) { pruneLeafGST(t->gst, leaf); }

int sizeRBT(RBT *t) { return sizeGST(t->gst); }

void statisticsRBT(RBT *t, FILE *fp) { statisticsGST(t->gst, fp); }

void displayRBT(RBT *t, FILE *fp) { displayGST(t->gst, fp); }

int debugRBT(RBT *t, int level) { return debugGST(t->gst, level); }

void freeRBT(RBT *t) {
	freeGST(t->gst);
	free(t);
}

void *unwrapRBT(TNODE *n) {
	RBTVAL *value = unwrapGST(n);
	return value->value;
}

int freqRBT(RBT *t, void *key) {
	RBTVAL *rbtval = newRBTVAL(t, key);
	int freq = freqGST(t->gst, rbtval);
	free(rbtval);
	return freq;
}

int duplicatesRBT(RBT *t) { return duplicatesGST(t->gst); }