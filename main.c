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

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

GLFWwindow* window;

/* Triangle triangle(Triangle *source, Space *space) { */
/*   Triangle output; */
/*   output.a = mvmultiply(source->a, *space); */
/*   output.b = mvmultiply(source->b, *space); */
/*   output.c = mvmultiply(source->c, *space); */
  
/*   return output; */
/* } */

Triangle triangle(Triangle *source, Space *space) {
  float x = fabs(source->b.x - source->c.x);
  float y = fabs(source->a.y - source->c.y);
  Triangle t = *source;

  Triangle output;
  Vertex a = {source->c.x + x/2, source->c.y, 0};
  Vertex b = {source->c.x + x / 4, source->c.y + y/2, 0};
  Vertex c = {source->b.x - x / 4, source->c.y + y/2, 0};
  output.a = a;
  output.b = b;
  output.c = c;
  
  return output;
}


void vertices(Triangle *source, Space *space, int depth, float *output, size_t count) {
  if (depth == 0) {
      return;
  }
  Triangle tr = triangle(source, space);
  memcpy(output, (float *)&tr, sizeof(Triangle));
  Triangle array[3];
  
  Triangle first = {
    .a = source->a,
    .b = tr.c,
    .c = tr.b
  };
  array[0] = first;
  
  Triangle second = {
    .a = tr.c,
    .b = source->b,
    .c = tr.a
  };
  array[1] = second;

  Triangle third = {
    .a = tr.b,
    .b = tr.a,
    .c = source->c
  };
  array[2] = third;

  size_t subcount = (count - sizeof(Triangle) / sizeof(float)) / 3;
  
  for (int i = 1; i <= 3; i++) {
    vertices(&array[i - 1], space, depth - 1, &output[subcount * i], subcount);
  }
}

int main(void) 
{
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1080, 1080,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = 1;
  glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint program = reload_shaders();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t sof = sizeof(float);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  float step = M_PI / 256;
  
  int depth = 3;
  size_t triangles_count = 1 + 3 + 9;
  size_t output_size = sizeof(Triangle) * (triangles_count);
  float *output = malloc(output_size);

  Space rospace = mmultiply(z_rotated(make_space(1), M_PI), 0.5);
  Triangle src = make_even_triangle(1);
  vertices(&src, &rospace, depth, output, output_size / sizeof(float));

  for (int i = 0; should_close; i++) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBufferData(GL_ARRAY_BUFFER, output_size, output, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, triangles_count * 3);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
