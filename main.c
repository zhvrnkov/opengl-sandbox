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

static float verticess[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static Vertex translations[] = {
  /* 0,   0,   0, */
  /* 0.5, 0,   0, */
  /* 0,   0.5, 0, */
  /* 0,   0,   0.5, */
  /* 0,   0,   -0.5, */
  /* -0.5,   0,   -0.0, */
  /* 0,   -0.5, 0, */
  0.0f,  0.0f,  0.0f,
  2.0f,  5.0f, -15.0f,
  -1.5f, -2.2f, -2.5f,
  -3.8f, -2.0f, -12.3f,
  2.4f, -0.4f, -3.5f,
  -1.7f,  3.0f, -7.5f,
  1.3f, -2.0f, -2.5f,
  1.5f,  2.0f, -2.5f,
  1.5f,  0.2f, -1.5f,
  -1.3f,  1.0f, -1.5f
};

static Vertex rotationVectors[] = {
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
  1.0, 0.3, 0.5,
};

const float SCR_WIDTH = 1000;
const float SCR_HEIGHT = 1000;

int main(void) 
{
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = 1;
  glewInit();
  glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint program = reload_shaders();
  
  int objectIndexUniform = glGetUniformLocation(program, "object_index");
  int translationsUniform = glGetUniformLocation(program, "translations");
  int rotationVectorsUniform = glGetUniformLocation(program, "rotation_vectors");
  int rotationAnglesUniform = glGetUniformLocation(program, "rotation_angles");
  int cameraYAngleUniform = glGetUniformLocation(program, "camera_angle");

  uint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticess), verticess, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  int width, height, nrChannels;
  // stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("/home/vz/fun/opengl-tutorial/sandbox/container.jpg", &width, &height, &nrChannels, 0);
  if (!data) {
    fprintf(stderr, "no data\n");
  }
  uint texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  
  size_t translations_count = sizeof(translations) / sizeof(Vertex);
  glUniform3fv(translationsUniform, translations_count, (float *)translations);
  glUniform3fv(rotationVectorsUniform, translations_count, (float *)rotationVectors);

  float step = M_PI / 1024;
  float *angles = (float *)malloc(translations_count * sizeof(float));
  
  for (int i = 0; should_close; i++) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1f(cameraYAngleUniform, step * i);
    
    for (int j = 0; j < translations_count; j++) {
      angles[j] = step * i * (j + 1);
      glUniform1fv(rotationAnglesUniform, translations_count, angles);

      glUniform1i(objectIndexUniform, j);

      glDrawArrays(GL_TRIANGLES, 0, sizeof(verticess) / sizeof(float));
    }
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}
