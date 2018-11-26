#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cda.h"

struct queue {
  CDA *array;
  int debug;
  void (*display)(void *, FILE *);
};

QUEUE *newQUEUE(void) {
  QUEUE *items = malloc(sizeof(QUEUE));
  assert(items != 0);
  items->array = newCDA();
  items->debug = 0;
  return items;
}

void setQUEUEdisplay(QUEUE *items, void (*d)(void *, FILE *)) {
  items->display = d;
  setCDAdisplay(items->array, d);
}

void setQUEUEfree(QUEUE *items, void (*f)(void *)) {
  setCDAfree(items->array, f);
}

void enqueue(QUEUE *items, void *value) { insertCDAback(items->array, value); }

void *dequeue(QUEUE *items) { return removeCDAfront(items->array); }

void *peekQUEUE(QUEUE *items) {
  assert(sizeCDA(items->array) > 0);
  return getCDA(items->array, 0);
}

void displayQUEUE(QUEUE *items, FILE *fp) {
  if (items->debug == 0) {
    fprintf(fp, "<");
    for (int i = 0; i < sizeCDA(items->array); i++) {
      if (items->display) items->display(getCDA(items->array, i), fp);
      else fprintf(fp, "@%p",getCDA(items->array, i));
      if (i != sizeCDA(items->array) - 1) fprintf(fp, ",");
    }
    fprintf(fp, ">");
  } else {
    debugCDA(items->array, items->debug - 1);
    displayCDA(items->array, fp);
  }
}

int debugQUEUE(QUEUE *items, int level) {
  int temp = items->debug;
  items->debug = level;
  return temp;
}

void freeQUEUE(QUEUE *items) {
  freeCDA(items->array);
  free(items);
}

int sizeQUEUE(QUEUE *items) { return sizeCDA(items->array); }