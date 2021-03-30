// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "common/linear_algebra.h"
#include "shaders/setup.h"

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

int main(void)
{
	// Initialise GLFW
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1080, 1080,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = 1; // Needed for core profile
  glewInit();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint VAOs[2];
	glGenVertexArrays(2, (GLuint *)&VAOs);
	GLuint VAO = VAOs[0];
  GLuint VAO2 = VAOs[1];

	// Create and compile our GLSL program from the shaders
  GLuint program = reload_shaders();

  GLuint VBOs[2];
	glGenBuffers(2, (GLuint *)&VBOs);
  GLuint VBO = VBOs[0];
  GLuint VBO2 = VBOs[1];
  
  GLuint EBO;
  glGenBuffers(1, &EBO);

  float vertices[] = {
     1,  0, 0,
     0,  1, 0,
     0, -1, 0,
    -1,  0, 0
  };
  
  unsigned int indices[] = {
    0, 1, 3,
    0, 2, 3,
  };

	glBindVertexArray(VAO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  /* glBindVertexArray(VAO2); */
  /* glBindBuffer(GL_ARRAY_BUFFER, VBO2); */
  /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0); */
  /* glBufferData(GL_ARRAY_BUFFER, count, rtriangle, GL_STATIC_DRAW); */
  /* glEnableVertexAttribArray(0); */

  for(int i = 1; should_close; i++) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* glDisableVertexAttribArray(0); */
    /* glDisableVertexAttribArray(1); */

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

	// Cleanup VBO
	glDeleteBuffers(2, (GLuint *)&VBOs);
	glDeleteVertexArrays(2, (GLuint *)&VAOs);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

