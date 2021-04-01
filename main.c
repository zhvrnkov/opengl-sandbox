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


void vertices(Triangle source, Space *space, int depth, Triangle *output, int *index) {
  if (depth == 0) {
      return;
  }
  Triangle tr = triangle(source, space);
  output[*index] = tr;
  (*index)++;
  
  Triangle first = {
    .a = source.a,
    .b = tr.c,
    .c = tr.b
  };
  
  Triangle second = {
    .a = tr.c,
    .b = source.b,
    .c = tr.a
  };

  Triangle third = {
    .a = tr.b,
    .b = tr.a,
    .c = source.c
  };

  vertices(first, space, depth - 1, output, index);
  vertices(second, space, depth - 1, output, index);
  vertices(third, space, depth - 1, output, index);
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
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  int angleLocation = glGetUniformLocation(program, "angle");
  float step = M_PI / 128;
  
  int depth = 5;
  size_t tc = counter(depth);
  size_t output_size = tc * sizeof(Triangle);
  Triangle *output;
  output = malloc(counter(15) * sizeof(Triangle));

  Space rospace = mmultiply(z_rotated(make_space(1), M_PI), 0.5);
  Triangle src = make_even_triangle(1);
  int index = 0;
  vertices(src, &rospace, depth, output, &index);
  glBufferData(GL_ARRAY_BUFFER, output_size, (float *)output, GL_STATIC_DRAW);

  Vertex c1 = {1, 0, 0};
  Vertex c2 = {0, 1, 0};
  Vertex c3 = {0, 0, 1};

  for (int i = 0; should_close; i++) {
    glUniform1f(angleLocation, step * i);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, tc * 3);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
