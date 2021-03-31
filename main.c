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

Triangle triangle(Triangle *source, Space *space) {
  Triangle output;
  output.a = transformed(source->a, *space);
  output.b = transformed(source->b, *space);
  output.c = transformed(source->c, *space);
  
  return output;
}

/* void vertices(Triangle source, int depth, float *output) {   */
/*   Triangle tr = triangle(source, (Vertex *)source); */

/*   Vertex first[] = { */
/*     source[0], */
/*     tr[0], */
/*     tr[2] */
/*   }; */
  
/*   Vertex second[] = { */
/*     source[1], */
/*     tr[0], */
/*     tr[1] */
/*   }; */
  
/*   Vertex third[] = { */
/*     source[2], */
/*     tr[1], */
/*     tr[2] */
/*   }; */

/*   size_t triangle_size = sizeof(float) * 9; */
/*   memcpy(output, third, triangle_size); */
/*   memcpy(&output[9], second, triangle_size); */
/*   memcpy(&output[18], first, triangle_size); */
/*   free(tr); */
/* } */

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

  size_t triangles_count = 10;
  size_t output_size = sizeof(Triangle) * triangles_count;
  float *output = malloc(output_size);

  Space rospace = mmultiply(z_rotated(make_space(), M_PI), 0.5);
  Triangle src = make_even_triangle(1);
  memcpy(output, &src, sizeof(Triangle));

  for (int i = 1; i < triangles_count; i++) {
    Triangle tr = triangle(&src, &rospace);
    memcpy(&output[i * 9], &tr, sizeof(Triangle));
    src = tr;
  }

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
