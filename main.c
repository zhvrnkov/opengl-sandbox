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

Vertex *triangle(Vertex *source) {
  Vertex src_top_left = source[0];
  Vertex src_top_right = source[1];
  Vertex src_bottom = source[2];
  float xlength = src_top_right.x - src_top_left.x;
  float ylength = src_top_right.y - src_bottom.y;

  Vertex top;
  top.x = src_bottom.x;
  top.y = src_top_left.y;
  top.z = 0;
  
  Vertex bl;
  bl.x = src_bottom.x + ylength / 3;
  bl.y = src_bottom.y + ylength / 2;
  bl.z = 0;
  
  Vertex br;
  br.x = src_bottom.x - ylength / 3;
  br.y = src_bottom.y + ylength / 2;
  br.z = 0;

  Vertex *output = malloc(sizeof(Vertex) * 3);
  output[0] = top;
  output[1] = bl;
  output[2] = br;
  return output;
}

void vertices(Vertex *source, int depth, float *output) {  
  Vertex *tr = triangle((Vertex *)source);

  Vertex first[] = {
    source[0],
    tr[0],
    tr[2]
  };
  
  Vertex second[] = {
    source[1],
    tr[0],
    tr[1]
  };
  
  Vertex third[] = {
    source[2],
    tr[1],
    tr[2]
  };

  size_t triangle_size = sizeof(float) * 9;
  memcpy(output, third, triangle_size);
  memcpy(&output[9], second, triangle_size);
  memcpy(&output[18], first, triangle_size);
  free(tr);
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

  size_t triangle_size = sizeof(float) * 9;
  size_t output_size = triangle_size * 3;
  float *output = malloc(output_size);

  Vertex source[] = {
    -1,  1, 0,
    1,  1, 0,
    0, -1, 0
  };

  for (int i = 0; should_close; i++) {
    glClear(GL_COLOR_BUFFER_BIT);

    vertices(source, 1, output);
    /* Vertex *t = (Vertex *)output; */
    /* for (int i = 0; i < 6; i++) { */
    /*   print_vertex(t[i]); */
    /* } */
    /* return 0; */
    glBufferData(GL_ARRAY_BUFFER, output_size, output, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
