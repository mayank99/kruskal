#include "da.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct da {
  int size;                         // logical size
  int capacity;                     // maximum size
  void **store;                     // the actual array
  int debug;                        // debug flag for displayDA
  void (*release)(void *);          // free item
  void (*display)(void *, FILE *);  // display item
};

DA *newDA() {
  DA *items = malloc(sizeof(DA));
  assert(items != 0);
  items->debug = 0;
  items->size = 0;
  items->capacity = 1;
  items->store = malloc(sizeof(void *));
  return items;
}

void setDAdisplay(DA *items, void (*d)(void *, FILE *)) { items->display = d; }

void setDAfree(DA *items, void (*f)(void *)) { items->release = f; }

void insertDA(DA *items, int index, void *value) {
  assert(index >= 0 && index <= items->size);
  if (items->size == items->capacity) {
    items->capacity *= 2;
    items->store = realloc(items->store, items->capacity * sizeof(value));
  }
  assert(items->store != 0);

  for (int i = items->size; i > index; i--) items->store[i] = items->store[i - 1];
  items->store[index] = value;
  items->size += 1;
}

void *removeDA(DA *items, int index) {
  assert(items->size > 0);
  void *value = items->store[index];
  for (int i = index; i < items->size - 1; i++) items->store[i] = items->store[i + 1];
  items->size -= 1;

  if (items->size < 0.25 * items->capacity) {
    items->capacity = items->size == 0 ? 1 : items->capacity / 2;
    items->store = realloc(items->store, items->capacity * sizeof(value));
  }
  return value;
}

void unionDA(DA *recipient, DA *donor) {
  for (int i = 0; i < sizeDA(donor); i++) {
    insertDA(recipient, sizeDA(recipient), getDA(donor, i));
  }
  donor->store = realloc(donor->store, sizeof(void *));
  donor->size = 0;
  donor->capacity = 1;
}

void *getDA(DA *items, int index) {
  assert(index >= 0 && index < items->size);
  return items->store[index];
}

void *setDA(DA *items, int index, void *value) {
  assert(index >= 0 && index <= items->size);
  if (index == items->size) {
    insertDA(items, index, value);
    return 0;
  } else {
    void *temp = items->store[index];
    items->store[index] = value;
    return temp;
  }
}

int sizeDA(DA *items) { return items->size; }

void displayDA(DA *items, FILE *fp) {
  fprintf(fp, "[");
  for (int i = 0; i < items->size; i++) {
    if (items->display != 0) {
      items->display(items->store[i], fp);
    } else {
      fprintf(fp, "@%p", items->store[i]);
    }
    if (i != items->size - 1) fprintf(fp, ",");
  }
  if (items->debug > 0) {
    if (items->size != 0) fprintf(fp, ",");
    fprintf(fp, "[%d]", items->capacity - items->size);
  }
  fprintf(fp, "]");
}

int debugDA(DA *items, int level) {
  int temp = items->debug;
  items->debug = level;
  return temp;
}

void freeDA(DA *items) {
  if (items->release != 0) {
    for (int i = 0; i < items->size; i++) {
      items->release(items->store[i]);
    }
  }
  free(items->store);
  free(items);
}