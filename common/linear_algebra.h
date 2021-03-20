#ifndef linear_algebra_header
#define linear_algebra_header

#define COMPONENTS_COUNT 3
#define VECTOR_SIZE sizeof(float) * COMPONENTS_COUNT

void set_matrix(float **vectors, int vectors_count, float *output);
float *make_matrix(float **vectors, int vectors_count);
void set_sum(float *vec1, float *vec2, float *vec3, float *output);
float *sum(float *vec1, float *vec2, float *vec3);
void set_multiply(float *vector, float scalar, float *output);
float *multiply(float *vector, float scalar);
void set_transformed(float *vector, float **space, float *output);
float *transformed(float *vector, float **space);

#endif
