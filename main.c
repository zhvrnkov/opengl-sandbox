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

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t sof = sizeof(float);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sof, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sof, (void *)(3 * sof));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sof, (void *)(6 * sof));
  glEnableVertexAttribArray(2);

  float space[] = {
    1, 0, 0,
    0, 2, 0,
    0, 0, 1
  };
  int vertexSpaceLocation = glGetUniformLocation(program, "space");
  glUniformMatrix3fv(vertexSpaceLocation, 1, GL_FALSE, (float *)space);

  float triangle[] = {
     0, 1, 0, 1, 0, 0, 0,
     1, 0, 0, 0, 1, 0, 0,
    -1, 0, 0, 0, 0, 1, 0
  };

  float step = M_PI / 256;

  for (int i = 0; should_close; i++) {
    glClear(GL_COLOR_BUFFER_BIT);
    float angle = step * i;
    triangle[6] = triangle[13] = triangle[20] = angle;
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
