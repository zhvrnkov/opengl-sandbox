#include "linear_algebra.h"

#include <stdlib.h>
#include <math.h>

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

Space mmultiply(Space space, float scalar) {
  Space output = space;
  float *t = (float *)&output;
  for (int i = 0; i < sizeof(Space) / sizeof(float); i++) {
    t[i] *= scalar;
  }
  return output;
}

Vertex transformed(Vertex vector, Space space) {
  Vertex ts[3];
  ts[0] = multiply(space.x, vector.x);
  ts[1] = multiply(space.y, vector.y);
  ts[2] = multiply(space.z, vector.z);

  return sum(ts, 3);
}

Vertex transformeds(Vertex vector, Space *spaces, size_t space_count) {
  Vertex output = vector;
  for (int i = 0; i < space_count; i++) {
    output = transformed(output, spaces[i]);
  }
  return output;
}

Space make_space() {
  Space space;
  space.x.x = 1;
  space.x.y = 0;
  space.x.z = 0;

  space.y.x = 0;
  space.y.y = 1;
  space.y.z = 0;

  space.z.x = 0;
  space.z.y = 0;
  space.z.z = 1;

  return space;
}

Space make_rospace() {
  Space space;
  space.x.x = -0.5;
  space.x.y = 0;
  space.x.z = 0;

  space.y.x = 0;
  space.y.y = -0.5;
  space.y.z = 0;

  space.z.x = 0;
  space.z.y = 0;
  space.z.z = -0.5;

  return space;
}

Space x_rotated(Space space, double angle) {
  double angle_between_axis = M_PI / 2;
  Space output = space;

  double y_angle = angle;
  double z_angle = angle_between_axis + angle;

  output.y.x = cos(y_angle);
  output.y.z = sin(y_angle);

  output.z.x = cos(z_angle);
  output.z.z = sin(z_angle);

  return output;  
}

Space z_rotated(Space space, double angle) {
  double angle_between_axis = M_PI / 2;
  Space output = space;

  double x_angle = angle;
  double y_angle = angle_between_axis + angle;

  output.x.x = cos(x_angle);
  output.x.y = sin(x_angle);

  output.y.x = cos(y_angle);
  output.y.y = sin(y_angle);

  return output;
}

Space y_rotated(Space space, double angle) {
  double angle_between_axis = M_PI / 2;
  Space output = space;

  double x_angle = angle;
  double z_angle = angle_between_axis + angle;

  output.x.x = cos(x_angle);
  output.x.z = sin(x_angle);

  output.z.x = cos(z_angle);
  output.z.z = sin(z_angle);

  return output;  
}

Triangle make_even_triangle(float scale) {
  Space space = mmultiply(z_rotated(make_space(), 2 * M_PI / 3), scale);

  Vertex point1;
  point1.x = 0;
  point1.y = 1;
  point1.z = 0;

  Vertex point2 = transformed(point1, space);
  Vertex point3 = transformed(point2, space);
  Triangle tr = {
    .a = point1,
    .b = point2,
    .c = point3
  };
  return tr;
}
