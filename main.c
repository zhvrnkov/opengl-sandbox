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

int main( void )
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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
  reload_shaders();

  GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

  size_t points_size = 9 * sizeof(float);
  float *triangle = (float *)malloc(points_size);

  float p1[] = {0, 0, 0};
  float p2[] = {0.1, 0.9, 0};
  float p3[] = {0.9, 0.1, 0};

  float **tvectors = malloc(3 * sizeof(float*));
  float *tp1 = malloc(VECTOR_SIZE);
  float *tp2 = malloc(VECTOR_SIZE);
  float *tp3 = malloc(VECTOR_SIZE);
  tvectors[0] = tp1;
  tvectors[1] = tp2;
  tvectors[2] = tp3;

  float **space = malloc(3 * sizeof(float*));
  float *x_vector = malloc(VECTOR_SIZE);
  float *y_vector = malloc(VECTOR_SIZE);
  float *z_vector = malloc(VECTOR_SIZE);
  z_vector[0] = 0;
  z_vector[1] = 0;
  z_vector[2] = 0;

  space[0] = x_vector;
  space[1] = y_vector;
  space[2] = z_vector;

  double step = M_PI / 256;
  
  for(int i = 0; should_close; i++) {
    double x_angle = step * i;
    double y_angle = x_angle + M_PI / 2;

    x_vector[0] = cos(x_angle);
    x_vector[1] = sin(x_angle);
    x_vector[2] = 0;

    y_vector[0] = cos(y_angle);
    y_vector[1] = sin(y_angle);
    y_vector[2] = 0;

    set_transformed(p1, space, tp1);
    set_transformed(p2, space, tp2);
    set_transformed(p3, space, tp3);

    set_matrix(tvectors, 3, triangle);

    glBufferData(GL_ARRAY_BUFFER, points_size, triangle, GL_STATIC_DRAW);
    glClear(GL_COLOR_BUFFER_BIT);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

	// Cleanup VBO
	glDeleteBuffers(1, &buffer);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

