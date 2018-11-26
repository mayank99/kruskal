#ifndef __VERTEX_INCLUDED__
#define __VERTEX_INCLUDED__

#include <stdio.h>
#include "da.h"

typedef struct vertex VERTEX;
typedef struct neighbor NEIGHBOR;

struct vertex {
  int v;
  // void *owner;
  int flag;
  int key;
  int level;
  VERTEX *predecessor;
  DA *neighbors;
  // DA *weights;
  // DA *edges;
};

struct neighbor {
  VERTEX *vertex;
  int weight;
  int active;
};

extern VERTEX *newVERTEX(int);
extern NEIGHBOR *newNEIGHBOR(VERTEX *, int, int);
extern int indexVERTEXneighbor(VERTEX *, VERTEX *);
extern void displayVERTEX(void *, FILE *fp);
extern void setNEIGHBORactive(NEIGHBOR *, int);
extern int compareVERTEX(const void *, const void *);
extern int compareNEIGHBOR(const void *, const void *);
extern void freeVERTEX(void *);
#endif