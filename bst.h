#ifndef __BST_INCLUDED__
#define __BST_INCLUDED__

#include <stdio.h>
#include "tnode.h"

typedef struct bst BST;

extern BST *newBST(int (*c)(void *, void *));
extern void setBSTdisplay(BST *t, void (*d)(void *, FILE *));
extern void setBSTswapper(BST *t, void (*s)(TNODE *, TNODE *));
extern void setBSTfree(BST *t, void (*)(void *));
extern TNODE *getBSTroot(BST *t);
extern void setBSTroot(BST *t, TNODE *replacement);
extern void setBSTsize(BST *t, int s);
extern TNODE *insertBST(BST *t, void *value);
extern void *findBST(BST *t, void *key);
extern TNODE *locateBST(BST *t, void *key);
extern int deleteBST(BST *t, void *key);
extern TNODE *swapToLeafBST(BST *t, TNODE *node);
extern void pruneLeafBST(BST *t, TNODE *leaf);
extern int sizeBST(BST *t);
extern void statisticsBST(BST *t, FILE *fp);
extern void displayBST(BST *t, FILE *fp);
extern int debugBST(BST *t, int level);
extern void freeBST(BST *t);
#endif