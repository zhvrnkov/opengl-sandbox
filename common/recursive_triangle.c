#include "linear_algebra.h"
#include <math.h>

Triangle triangle(Triangle source, Space *space) {
  float x = fabs(source.b.x - source.c.x);
  float y = fabs(source.a.y - source.c.y);

  Triangle output;
  Vertex a = {source.c.x + x/2, source.c.y, 0};
  Vertex b = {source.c.x + x / 4, source.c.y + y/2, 0};
  Vertex c = {source.b.x - x / 4, source.c.y + y/2, 0};
  output.a = a;
  output.b = b;
  output.c = c;
  
  return output;
}

void vertices(Triangle source, 
              Vertex center,
              int depth, 
              Triangle *output, 
              Triangle *centers, 
              int *index) {
  if (depth == 0) {
      return;
  }
  Space space = make_space(0.5);
  Triangle tr = triangle_multiply(source, space);
  output[*index] = tr;
  centers[*index].a = center;
  centers[*index].b = center;
  centers[*index].c = center;
  (*index)++;
  
  Triangle next = triangle_multiply(tr, space);

  float tr_x = fabs(tr.b.x - tr.c.x);
  float tr_y = fabs(tr.a.y - tr.b.y);

  float next_x = fabs(next.b.x - next.c.x);
  float next_y = fabs(next.a.y - next.b.y);

  Vertex first = {
    .x = center.x,
    .y = center.y + tr_y / 2 + next_y / 3,
    .z = center.z
  };
  Vertex second = {
    .x = center.x + tr_x / 2,
    .y = center.y - tr_y / 2 + next_y / 3,
    .z = center.z
  };
  Vertex third = {
    .x = center.x - tr_x / 2,
    .y = center.y - tr_y / 2 + next_y / 3,
    .z = center.z
  };

  vertices(tr, first, depth - 1, output, centers, index);
  vertices(tr, second, depth - 1, output, centers, index);
  vertices(tr, third, depth - 1, output, centers, index);
}

int counter(int depth) {
  if (depth == 0) {
    return 0;
  }
  else {
    return pow(3, depth - 1) + counter(depth - 1);
  }
}
