#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "da.c"
#include "edge.h"
#include "integer.h"
#include "rbt.h"
#include "scanner.h"
#include "set.h"
#include "vertex.h"

// forward declarations for methods
int storeEdges(FILE *fp, DA *da);
VERTEX **adjacencyList(DA *edges, int size);
SET *makeSets(VERTEX **vertices, int size);
void displayMST(VERTEX *prev);

// main method for running Kruskal's algo
int main(int argc, char **argv) {
  // process args
  if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    printf("Written by Mayank Agarwal\n");
    return 0;
  }

  // read edges from input file
  FILE *graph = fopen(argv[1], "rb");
  DA *edgesDA = newDA();
  int maxVertex = storeEdges(graph, edgesDA);
  fclose(graph);

  // get vertices and make a disjoint set
  VERTEX **vertices = adjacencyList(edgesDA, maxVertex + 1);
  SET *set = makeSets(vertices, maxVertex + 1);

  // sort edges by weight
  void **edges = edgesDA->store;
  int sizeEdges = sizeDA(edgesDA);
  // free(edgesDA);
  qsort(edges, sizeEdges, sizeof(edges[0]), compareEDGEweight);

  // find MST using Kruskal's algorithm
  for (int i = 0; i < sizeEdges; i++) {
    EDGE *e = edges[i];
    VERTEX *a = vertices[e->v1];
    VERTEX *b = vertices[e->v2];
    int x = findSET(set, a->key);
    int y = findSET(set, b->key);
    if (x != y) {
      a->flag = 0;
      b->flag = 0;
      setNEIGHBORactive(getDA(a->neighbors, indexVERTEXneighbor(a, b)), 1);
      setNEIGHBORactive(getDA(b->neighbors, indexVERTEXneighbor(b, a)), 1);
      unionSET(set, x, y);
    }
  }

  // display the MST using BFS
  VERTEX *start = 0;
  for (int i = 0; i <= maxVertex; i++) {
    start = vertices[i];
    if (start && start->flag == 0) break;
  }
  displayMST(start);

  // free everything
  setDAfree(edgesDA, free);
  setSETfree(set, freeVERTEX);
  freeSET(set);
  for (int i = 0; i <= maxVertex; i++) if (vertices[i]) freeVERTEX(vertices[i]);
  free(vertices);
  freeDA(edgesDA);
}

// reads edges from the input file and stores it in DA
int storeEdges(FILE *fp, DA *da) {
  int maxVertex = -1;
  RBT *rbt = newRBT(compareEDGE);

  do {
    // read in vertices
    int v1 = readInt(fp);
    int v2 = readInt(fp);
    if (v2 > v1) { v1 = v1 + v2; v2 = v1 - v2; v1 = v1 - v2; }
    if (v1 > maxVertex) maxVertex = v1;

    // read in weight
    char *w = readToken(fp);
    if (w == 0) break;
    int weight = atoi(w);
    if (weight == 0) {
      weight = 1;
    } else {
      free(w);
      w = readToken(fp);  // semicolon
    }
    free(w);

    // store edges
    EDGE *newEdge = newEDGE(v1, v2, weight);
    EDGE *existingEdge = findRBT(rbt, newEdge);
    if (!existingEdge) {
      insertRBT(rbt, newEdge);
      insertDAback(da, newEdge);
    } else {
      existingEdge->weight = newEdge->weight;
      free(newEdge);
    }
  } while (!feof(fp));

  freeRBT(rbt);
  return maxVertex;
}

// builds an adjacency list from a list of edges
VERTEX **adjacencyList(DA *edges, int size) {
  VERTEX **vertices = calloc(size, sizeof(VERTEX *));
  for (int i = 0, len = sizeDA(edges); i < len; i++) {
    EDGE *e = getDA(edges, i);
    if (!vertices[e->v1]) vertices[e->v1] = newVERTEX(e->v1);
    if (!vertices[e->v2]) vertices[e->v2] = newVERTEX(e->v2);
    VERTEX *a = vertices[e->v1];
    VERTEX *b = vertices[e->v2];
    insertDAback(a->neighbors, newNEIGHBOR(b, e->weight, 0));
    insertDAback(b->neighbors, newNEIGHBOR(a, e->weight, 0));
  }
  return vertices;
}

// performs make-sets on all verties
SET *makeSets(VERTEX **vertices, int size) {
  SET *set = newSET();
  for (int i = 0; i < size; i++) {
    if (vertices[i]) {
      int index = makeSET(set, vertices[i]);
      vertices[i]->key = index;
    }
  }
  return set;
}

// displays the MST using BFS
void displayMST(VERTEX *prev) {
  DA *queue = newDA();
  int weight = 0;
  prev->flag = 1;
  prev->level = 0;
  printf("%d:", prev->level);
  insertDAback(queue, prev);

  while (sizeDA(queue) > 0) {
    VERTEX *vertex = removeDA(queue, 0);
    printf(prev->level == vertex->level ? " " : "\n%d: ", vertex->level);
    displayVERTEX(vertex, stdout);
    prev = vertex;

    // enqueue unvisited neighbors
    for (int i = 0; i < sizeDA(vertex->neighbors); i++) {
      NEIGHBOR *neighbor = getDA(vertex->neighbors, i);  // neighbors[i];
      if (neighbor->vertex->flag == 0 && neighbor->active) {
        neighbor->vertex->flag = neighbor->weight;
        neighbor->vertex->level = vertex->level + 1;
        neighbor->vertex->predecessor = vertex;
        insertDAback(queue, neighbor->vertex);
        weight += neighbor->weight;
      }
    }

    // sort queue at levels
    void **store = queue->store;
    int size = sizeDA(queue);
    qsort(store, size, sizeof(void *), compareVERTEX);
  }

  // print total weight
  printf("\nweight: %d\n", weight);

  // Clear the queue when done
  free(queue->store);
  free(queue);
}