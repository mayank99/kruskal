#ifndef __QUEUE_INCLUDED__
#define __QUEUE_INCLUDED__

#include <stdio.h>

typedef struct queue QUEUE;

extern QUEUE *newQUEUE(void);
extern void setQUEUEdisplay(QUEUE *, void (*)(void *, FILE *));
extern void setQUEUEfree(QUEUE *, void (*)(void *));
extern void enqueue(QUEUE *items, void *value);
extern void *dequeue(QUEUE *items);
extern void *peekQUEUE(QUEUE *items);
extern void displayQUEUE(QUEUE *items, FILE *fp);
extern int debugQUEUE(QUEUE *items, int level);
extern void freeQUEUE(QUEUE *items);
extern int sizeQUEUE(QUEUE *items);

#endif