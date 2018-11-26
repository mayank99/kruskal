#ifndef __RBT_INCLUDED__
#define __RBT_INCLUDED__

#include <stdio.h>
#include "tnode.h"

typedef struct rbt RBT;

extern RBT *newRBT(int (*c)(void *, void *));
extern void setRBTdisplay(RBT *t, void (*d)(void *, FILE *));
extern void setRBTswapper(RBT *t, void (*s)(TNODE *, TNODE *));
extern void setRBTfree(RBT *t, void (*)(void *));
extern TNODE *getRBTroot(RBT *t);
extern void setRBTroot(RBT *t, TNODE *replacement);
extern void setRBTsize(RBT *t, int s);
extern TNODE *insertRBT(RBT *t, void *value);
extern void *findRBT(RBT *t, void *key);
extern TNODE *locateRBT(RBT *t, void *key);
extern int deleteRBT(RBT *t, void *key);
extern TNODE *swapToLeafRBT(RBT *t, TNODE *node);
extern void pruneLeafRBT(RBT *t, TNODE *leaf);
extern int sizeRBT(RBT *t);
extern void statisticsRBT(RBT *t, FILE *fp);
extern void displayRBT(RBT *t, FILE *fp);
extern int debugRBT(RBT *t, int level);
extern void freeRBT(RBT *t);
extern void *unwrapRBT(TNODE *n);
extern int freqRBT(RBT *g, void *key);
extern int duplicatesRBT(RBT *g);

#endif