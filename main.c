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
#include "common/recursive_triangle.h"
#include "shaders/setup.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

GLFWwindow* window;

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
