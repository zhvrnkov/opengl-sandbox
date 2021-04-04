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
              Space *space, 
              int depth, 
              Triangle *output, 
              Vertex *centers, 
              int *index) {
  if (depth == 0) {
      return;
  }
  Triangle tr = triangle_multiply(source, half_space);
  output[*index] = tr;
  centers[*index] = triangle_center(source);
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

  vertices(first, space, depth - 1, output, centers, index);
  vertices(second, space, depth - 1, output, centers, index);
  vertices(third, space, depth - 1, output, centers, index);
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
  int width, height, nrChannels;
  unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); 

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
  Vertex *centers;
  centers = malloc(counter(15) * sizeof(Vertex));

  Triangle src = make_even_triangle(1);
  Space rospace = mmultiply(z_rotated(make_space(1), M_PI), 0.5);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  /* Vertex c1 = {1, 0, 0}; */
  /* Vertex c2 = {0, 1, 0}; */
  /* Vertex c3 = {0, 0, 1}; */

  int depth = 1;
  size_t tc = counter(depth);
  int direction = 1;
  int max_depth = 10;

  Triangle test;
  Vertex a = {0.7, 1, 0};
  Vertex b = {0.95 , 0.5, 0};
  Vertex c = {0.45, 0.5, 0};
  test.a = a;
  test.b = b;
  test.c = c;

  Vertex center[3];
  Vertex cen = triangle_center(test);
  center[0] = cen;
  center[1] = cen;
  center[2] = cen;

  Vertex na = {0, 0.1, 0};
  Vertex nb = {0.1, -0.1, 0};
  Vertex nc = {-0.1, -0.1, 0};
  test.a = na;
  test.b = nb;
  test.c = nc;

//  print_vertex(center);


  for (int i = 0; should_close; i++) {
    /* if (i % 5 == 0) { */
    /*   int new_depth = (depth + 1) % 6; */
    /*   if (depth >= max_depth) { */
    /*     direction = -1; */
    /*   } */
    /*   else if (depth <= 1) { */
    /*     direction = 1; */
    /*   } */
    /*   depth = (depth + direction); */
    /*   tc = counter(depth); */
    /*   size_t output_size = tc * sizeof(Triangle); */
    /*   size_t centers_size = tc * sizeof(Vertex); */

    /*   int index = 0; */
    /*   vertices(src, &rospace, depth, output, centers, &index); */

    /*   glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); */
    /*   glBufferData(GL_ARRAY_BUFFER, output_size, (float *)output, GL_STATIC_DRAW); */

    /*   glBindBuffer(GL_ARRAY_BUFFER, vbos[1]); */
    /*   glBufferData(GL_ARRAY_BUFFER, centers_size, (float *)centers, GL_STATIC_DRAW); */
    /* } */

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), (float *)&test, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, (float *)&center, GL_STATIC_DRAW);

    glUniform1f(angleLocation, step * i);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, tc * 3);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
