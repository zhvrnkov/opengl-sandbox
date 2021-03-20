#include "linear_algebra.h"

#include <stdlib.h>

void set_matrix(float **vectors, int vectors_count, float *output) {
  for (int vector_index = 0; vector_index < vectors_count; vector_index++) {
    for (int vector_component = 0; vector_component < COMPONENTS_COUNT; vector_component++) {
      int base = vector_index * COMPONENTS_COUNT;
      output[base + vector_component] = vectors[vector_index][vector_component];
    }
  }
}

float *make_matrix(float **vectors, int vectors_count) {
  int total_number_of_components = vectors_count * COMPONENTS_COUNT;
  float *output = malloc(total_number_of_components * sizeof(float));
  set_matrix(vectors, vectors_count, output);
  return output;
}

Vertex sum(Vertex *array, size_t count) {
  Vertex output = {0,0,0};
  for (int i = 0; i < count; i++) {
    output.x += array[i].x;
    output.y += array[i].y;
    output.z += array[i].z;
  }
  return output;
}

Vertex multiply(Vertex vector, float scalar) {
  Vertex output;
  output.x = vector.x * scalar;
  output.y = vector.y * scalar;
  output.z = vector.z * scalar;
  return output;
}

Vertex transformed(Vertex vector, Space space) {
  Vertex ts[3];
  ts[0] = multiply(space.x, vector.x);
  ts[1] = multiply(space.y, vector.y);
  ts[2] = multiply(space.z, vector.z);

  return sum(ts, 3);
}
