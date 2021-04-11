#ifndef recursive_triangle_header
#define recursive_triangle_header

Triangle triangle(Triangle source, Space *space);
void vertices(Triangle source, 
              Vertex center,
              int depth, 
              Triangle *output, 
              Triangle *centers, 
              int *index);
int counter(int depth);

#endif
