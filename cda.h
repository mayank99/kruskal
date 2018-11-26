#ifndef __CDA_INCLUDED__
#define __CDA_INCLUDED__

#include <stdio.h>

typedef struct cda CDA;

struct cda {
  int size;                         // logical size
  int capacity;                     // maximum size
  void **store;                     // the actual array
  int start;                        // index of the first/leftmost element
  int debug;                        // debug flag for displayCDA
  void (*release)(void *);          // free item
  void (*display)(void *, FILE *);  // display item
};

extern CDA *newCDA(void);
extern void setCDAdisplay(CDA *items, void (*display)(void *, FILE *));
extern void setCDAfree(CDA *items, void (*free)(void *));
extern void insertCDA(CDA *items, int index, void *value);
extern void *removeCDA(CDA *items, int index);
extern void unionCDA(CDA *recipient, CDA *donor);
extern void *getCDA(CDA *items, int index);
extern void *setCDA(CDA *items, int index, void *value);
extern int sizeCDA(CDA *items);
extern void displayCDA(CDA *, FILE *);
extern int debugCDA(CDA *, int level);
extern void freeCDA(CDA *);

#define insertCDAfront(items, value) insertCDA(items, 0, value)
#define insertCDAback(items, value) insertCDA(items, sizeCDA(items), value)
#define removeCDAfront(items) removeCDA(items, 0)
#define removeCDAback(items) removeCDA(items, sizeCDA(items) - 1)

#endif