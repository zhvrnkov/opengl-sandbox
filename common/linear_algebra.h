#ifndef linear_algebra_header
#define linear_algebra_header

#define COMPONENTS_COUNT 3
#define VECTOR_SIZE sizeof(float) * COMPONENTS_COUNT

#include <stdlib.h>

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  Vertex x, y, z;
} Space;

#define print_vertex(V) printf("x: %f, y: %f, z: %f\n", V)

void set_matrix(float **vectors, int vectors_count, float *output);
float *make_matrix(float **vectors, int vectors_count);
Vertex sum(Vertex *array, size_t count);
Vertex multiply(Vertex vector, float scalar);
Vertex transformed(Vertex vector, Space space);

#endif
