#include "cda.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

CDA *newCDA() {
  CDA *items = malloc(sizeof(CDA));
  assert(items != 0);
  items->debug = 0;
  items->size = 0;
  items->capacity = 1;
  items->store = malloc(sizeof(void *));
  items->start = 0;
  items->display = 0;
  items->release = 0;
  return items;
}

void setCDAdisplay(CDA *items, void (*d)(void *, FILE *)) { items->display = d; }

void setCDAfree(CDA *items, void (*f)(void *)) { items->release = f; }

void resize(CDA *items, double factor) {
  int capacity = factor == 2 ? items->capacity * 2 : items->capacity / 2;
  if (capacity == 0 || factor == 0) capacity = 1;
  void **temp = malloc(capacity * sizeof(void *));
  for (int i = 0; i < items->size; i++) temp[i] = getCDA(items, i);
  free(items->store);
  items->store = temp;
  items->start = 0;
  items->capacity = capacity;
}

void *setElement(CDA *items, int index, void *value) {
  index = (items->start + index + items->capacity) % items->capacity;
  void *temp = items->store[index];
  items->store[index] = value;
  return temp;
}

void insertCDA(CDA *items, int index, void *value) {
  // housekeeping / memory allocation
  assert(index >= 0 && index <= items->size);
  if (items->size == items->capacity) resize(items, 2);
  assert(items->store != 0);

  // shifting and inserting
  if (index == 0) {
    int start = (items->size == 0) ? 0 : (items->start - 1 + items->capacity) % items->capacity;
    items->store[start] = value;
    items->start = start;
  } else {
    if (index < items->size / 2) {
      for (int i = 0; i < index; i++) setElement(items, i - 1, getCDA(items, i));
      setElement(items, index - 1, value);
      items->start = (items->start - 1 + items->capacity) % items->capacity;
    } else {
      for (int i = items->size; i > index; i--) setElement(items, i, getCDA(items, i - 1));
      setElement(items, index, value);
    }
  }
  items->size += 1;
}

void *removeCDA(CDA *items, int index) {
  assert(items->size > 0);
  void *value = getCDA(items, index);

  // shifting elements
  if (index < items->size / 2) {
    for (int i = index; i > 0; i--) setElement(items, i, getCDA(items, i - 1));
    items->start = (items->start + 1) % items->capacity;
  } else {
    for (int i = index; i < items->size - 1; i++) setElement(items, i, getCDA(items, i + 1));
  }
  items->size -= 1;

  if (items->size < 0.25 * items->capacity) resize(items, items->size != 0 ? 0.5 : 0);
  return value;
}

void unionCDA(CDA *recipient, CDA *donor) {
  for (int i = 0; i < sizeCDA(donor); i++) {
    insertCDA(recipient, sizeCDA(recipient), getCDA(donor, i));
  }
  donor->store = realloc(donor->store, sizeof(void *));
  donor->size = 0;
  donor->capacity = 1;
}

void *getCDA(CDA *items, int index) {
  assert(index >= 0 && index < items->size);
  return items->store[(items->start + index) % items->capacity];
}

void *setCDA(CDA *items, int index, void *value) {
  assert(index >= -1 && index <= items->size);
  if (index == -1)
    insertCDAfront(items, value);
  else if (index == items->size)
    insertCDAback(items, value);
  else
    return setElement(items, index, value);
  return 0;
}

int sizeCDA(CDA *items) { return items->size; }

void displayCDA(CDA *items, FILE *fp) {
  fprintf(fp, "(");
  for (int i = 0; i < items->size; i++) {
    if (items->display != 0)
      items->display(getCDA(items, i), fp);
    else
      fprintf(fp, "@%p", getCDA(items, i));

    if (i != items->size - 1) fprintf(fp, ",");
  }
  if (items->debug > 0) {
    if (items->size != 0) fprintf(fp, ",");
    fprintf(fp, "(%d)", items->capacity - items->size);
  }
  fprintf(fp, ")");
}

int debugCDA(CDA *items, int level) {
  int temp = items->debug;
  items->debug = level;
  return temp;
}

void freeCDA(CDA *items) {
  if (items->release != 0) {
    for (int i = 0; i < items->size; i++) {
      items->release(getCDA(items, i));
    }
  }
  free(items->store);
  free(items);
}