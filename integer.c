#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "integer.h"

struct INTEGER
    {
    int value;
    };

INTEGER *
newINTEGER(int x)
    {
    INTEGER *p = malloc(sizeof(INTEGER));
    assert(p != 0);
    p->value = x;
    return p;
    }

int 
getINTEGER(INTEGER *v)
    {
    return v->value;
    }

int 
setINTEGER(INTEGER *v,int x)
    {
    int old = v->value;
    v->value = x;
    return old;
    }

void 
displayINTEGER(void *v,FILE *fp)
    {
    fprintf(fp,"%d",getINTEGER((INTEGER *) v));
    }

int 
compareINTEGER(void *v,void *w)
    {
    return getINTEGER(v) - getINTEGER(w);
    }

void
freeINTEGER(void *v)
    {
    free((INTEGER *) v);
    }
