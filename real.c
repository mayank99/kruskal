#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "real.h"

struct real
    {
    double value;
    };

REAL *
newREAL(double x)
    {
    REAL *p = malloc(sizeof(REAL));
    if (p == 0) { fprintf(stderr,"out of memory\n"); exit(1); }
    p->value = x;
    return p;
    }

double 
getREAL(REAL *v)
    {
    return v->value;
    }

double 
setREAL(REAL *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

void 
displayREAL(void *v, FILE *fp)
    {
    fprintf(fp,"%f",getREAL(v));
    }
    
int 
compareREAL(void *v,void *w)
    {
    return (int)getREAL(v) - (int)getREAL(w);
    }

void
freeREAL(void *v)
    {
    free((REAL *) v);
    }
