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

void set_sum(float *vec1, float *vec2, float *vec3, float *output) {
  for (int i = 0; i < COMPONENTS_COUNT; i++) {
    output[i] = vec1[i] + vec2[i] + vec3[i];
  }
}

float *sum(float *vec1, float *vec2, float *vec3) {
  float *output = malloc(VECTOR_SIZE);
  set_sum(vec1, vec2, vec3, output);
  return output;
}

void set_multiply(float *vector, float scalar, float *output) {
  for (int i = 0; i < COMPONENTS_COUNT; i++) {
    output[i] = vector[i] * scalar;
  }
}

float *multiply(float *vector, float scalar) {
  float *output = malloc(VECTOR_SIZE);
  set_multiply(vector, scalar, output);
  return output;
}

void set_transformed(float *vector, float **space, float *output) {
  float *transformed_x = multiply(space[0], vector[0]);
  float *transformed_y = multiply(space[1], vector[1]);
  float *transformed_z = multiply(space[2], vector[2]);

  set_sum(transformed_x, transformed_y, transformed_z, output);
  
  free(transformed_x);
  free(transformed_y);
  free(transformed_z);
}

float *transformed(float *vector, float **space) {
  float *transformed_x = multiply(space[0], vector[0]);
  float *transformed_y = multiply(space[1], vector[1]);
  float *transformed_z = multiply(space[2], vector[2]);

  float *output = sum(transformed_x, transformed_y, transformed_z);
  
  free(transformed_x);
  free(transformed_y);
  free(transformed_z);
  
  return output;
}
