#ifndef __TNODE_INCLUDED__
#define __TNODE_INCLUDED__

#include <stdio.h>

typedef struct tnode TNODE;

extern TNODE *newTNODE(void *v, TNODE *l, TNODE *r, TNODE *p);
extern void setTNODEdisplay(TNODE *n, void (*d)(void *value, FILE *fp));
extern void setTNODEfree(TNODE *n, void (*f)(void *value));
extern void *getTNODEvalue(TNODE *n);
extern void setTNODEvalue(TNODE *n, void *replacement);
extern TNODE *getTNODEleft(TNODE *n);
extern void setTNODEleft(TNODE *n, TNODE *replacement);
extern TNODE *getTNODEright(TNODE *n);
extern void setTNODEright(TNODE *n, TNODE *replacement);
extern TNODE *getTNODEparent(TNODE *n);
extern void setTNODEparent(TNODE *n, TNODE *replacement);
extern void displayTNODE(TNODE *n, FILE *fp);
extern int debugTNODE(TNODE *n, int level);
extern void freeTNODE(TNODE *n);

#endif