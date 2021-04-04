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

typedef struct {
  Vertex a, b, c;
} Triangle;

#define print_vertex(V) printf("x: %f, y: %f, z: %f\n", V.x, V.y, V.z)
#define print_tr(T) print_vertex(T.a); print_vertex(T.b); print_vertex(T.c); printf("\n");
#define print_sp(S) print_vertex(S.x); print_vertex(S.y); print_vertex(S.z); printf("\n");

void set_matrix(float **vectors, int vectors_count, float *output);
float *make_matrix(float **vectors, int vectors_count);
Vertex sum(Vertex *array, size_t count);
Vertex multiply(Vertex vector, float scalar);
Vertex transformed(Vertex vector, Space space);
Vertex transformeds(Vertex vector, Space *spaces, size_t space_count);
Space make_space(float scale);
Space make_rospace();
Space z_rotated(Space space, double angle);
Space y_rotated(Space space, double angle);
Space x_rotated(Space space, double angle);
Space mmultiply(Space, float);
Triangle make_even_triangle(float scale);
Vertex mvmultiply(Vertex vector, Space space);
Triangle shiftx(Triangle, float);
Triangle shifty(Triangle, float);
Vertex triangle_center(Triangle);
Triangle triangle_multiply(Triangle, Space);

#endif
