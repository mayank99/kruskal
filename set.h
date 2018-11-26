#ifndef __SET_INCLUDED__
#define __SET_INCLUDED__

#include <stdio.h>

typedef struct set SET;

extern SET *newSET(void);
extern void setSETdisplay(SET *s, void (*d)(void *, FILE *));
extern void setSETfree(SET *s, void (*d)(void *));
extern int makeSET(SET *d, void *value);
extern int findSET(SET *d, int index);
extern int unionSET(SET *d, int index1, int index2);
extern void displaySET(SET *d, FILE *fp);
extern int debugSET(SET *d, int level);
extern void freeSET(SET *d);

#endif