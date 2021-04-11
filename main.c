#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "common/linear_algebra.h"
#include "shaders/setup.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

GLFWwindow* window;

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

Space half_space;

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

int main(void) 
{
  half_space = mmultiply(z_rotated(make_space(1), M_PI), 0.5);
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1000, 1000,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = 1;
  glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint program = reload_shaders();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbos[2];
  glGenBuffers(2, vbos);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  int angleLocation = glGetUniformLocation(program, "angle");

  float step = M_PI / 64;
 
  Triangle *output;
  output = malloc(counter(15) * sizeof(Triangle));
  Triangle *centers;
  centers = malloc(counter(15) * sizeof(Triangle));

  Triangle src = make_evenr_triangle(1);
  Space space_123 = make_space(0.5);
  Triangle tr123 = triangle_multiply(src, space_123);
  // print_vertex(triangle_center(src));
  Vertex zero_origin = {
    .x = 0,
    .y = 0,
    .z = 0
  };

  int depth = 1;
  size_t tc = counter(depth);
  int direction = 1;
  int max_depth = 10;

  for (int i = 0; should_close; i++) {
    double value = sin(step * i);
    double zero = 0.0001;
    if (value > -zero && value < zero) {
      int direction_step = 1;//sin(step * i) == 0 ? 1 : 0;//(int)floor(sin(step * i));
      if (depth >= max_depth) {
        direction = -direction_step;
      }
      else if (depth <= 1) {
        direction = direction_step;
      }
      depth = (depth + direction);
      tc = counter(depth);
      size_t output_size = tc * sizeof(Triangle);
      size_t centers_size = tc * sizeof(Triangle);

      int index = 0;
      vertices(src, zero_origin, depth, output, centers, &index);

      glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
      glBufferData(GL_ARRAY_BUFFER, output_size, (float *)output, GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
      glBufferData(GL_ARRAY_BUFFER, centers_size, (float *)centers, GL_STATIC_DRAW);
    }

    glUniform1f(angleLocation, step * i);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, tc * 3);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
