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

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create and compile our GLSL program from the shaders
  GLuint program = reload_shaders();

  GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  size_t points_size = 9 * sizeof(float);
  Vertex triangle[3];

  Vertex p1 = {1, 0, 0};
  Vertex p2 = {0, 0.01, 0};
  Vertex p3 = {0.01, 0, 0};

  Space space = make_space();

  double step = M_PI / 256;
  
  for(int i = 1; should_close; i++) {
    double angle = step * i;
    Space x_rotated_space = x_rotated(space, angle);
    Space y_rotated_space = y_rotated(space, angle);
    Space z_rotated_space = z_rotated(space, angle * 1/6);
    size_t count = 2;
    Space spaces[] = {y_rotated_space, z_rotated_space};

    triangle[0] = transformeds(p1, spaces, count);
    triangle[1] = transformeds(p2, spaces, count);
    triangle[2] = transformeds(p3, spaces, count);
    
    glBufferData(GL_ARRAY_BUFFER, points_size, (float *)triangle, GL_STATIC_DRAW);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

	// Cleanup VBO
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

