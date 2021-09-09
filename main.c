#include <iostream>
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
// #include "common/recursive_triangle.h"
#include "shaders/setup.h"
#include "common/camera.h"

#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

void processInput(GLFWwindow *window);

GLFWwindow* window;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

vec3 translations[] = {
  vec3( 0.0f,  0.0f,  0.0f),
  vec3( 2.0f,  5.0f, -15.0f),
  vec3(-1.5f, -2.2f, -2.5f),
  vec3(-3.8f, -2.0f, -12.3f),
  vec3 (2.4f, -0.4f, -3.5f),
  vec3(-1.7f,  3.0f, -7.5f),
  vec3( 1.3f, -2.0f, -2.5f),
  vec3( 1.5f,  2.0f, -2.5f),
  vec3( 1.5f,  0.2f, -1.5f),
  vec3(-1.3f,  1.0f, -1.5f)
};

vec3 rotationVectors[] = {
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
  vec3(1.0, 0.3, 0.5),
};

vec3 lightTranslation = vec3(1.0f, 1.0f, -5.0f);

const float SCR_WIDTH = 1000;
const float SCR_HEIGHT = 1000;

vec3 cameraPos = vec3(0.0, 0.0, 10.0);
const vec3 initialCameraDirection = vec3(0.0, 0.0, -1.0);
vec3 cameraDirection = initialCameraDirection;
vec3 up = vec3(0.0, 1.0, 0.0);

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

  GLuint program = reload_shaders("./shaders/main.vert", "./shaders/main.frag", 0);
  GLuint lightProgram = reload_shaders("./shaders/main.lightning.vert", "./shaders/main.lightning.frag", 0);
  
  int modelUniform = glGetUniformLocation(program, "model");
  int viewUniform = glGetUniformLocation(program, "view");
  int projectionUniform = glGetUniformLocation(program, "projection");
  int timeUniform = glGetUniformLocation(program, "time");
  int objectColorUniform = glGetUniformLocation(program, "objectColor");
  int lightColorUniform = glGetUniformLocation(program, "lightColor");
  int lightPosUniform = glGetUniformLocation(program, "lightPos");

  int light_modelUniform = glGetUniformLocation(lightProgram, "model");
  int light_viewUniform = glGetUniformLocation(lightProgram, "view");
  int light_projectionUniform = glGetUniformLocation(lightProgram, "projection");
  /* int light_timeUniform = glGetUniformLocation(lightProgram, "time"); */

  uint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  int width, height, nrChannels;
  // stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
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

  float step = M_PI / 1024;
  float *angles = (float *)malloc(translations_count * sizeof(float));

  // LIGHT
  GLuint lightVao;

  glGenVertexArrays(1, &lightVao);
  
  glBindVertexArray(lightVao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // END OF LIGHT

  mat4 model;
  mat4 view;
  mat4 projection = make_projection_angle(45.0, 1, 0.1, 100);
  vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);
  vec3 lightColor = vec3(1.0f);

  float a = 0;
  for (int i = 0; should_close; i++) {

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightTranslation.x = cosf(a);
    lightTranslation.y = sinf(a);
    lightTranslation.z = sinf(a / 4) * 10 - 3.0;
    a += M_PI / 256;

    glUseProgram(lightProgram);
    glBindVertexArray(lightVao);
    model = make_model(lightTranslation, vec3(1.0f), 0.0f);
    glUniformMatrix4fv(light_modelUniform, 1, 0, (float *)&model[0][0]);
    glUniformMatrix4fv(light_viewUniform, 1, 0, (float *)&view[0][0]);
    glUniformMatrix4fv(light_projectionUniform, 1, 0, (float *)&projection[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));

    glUseProgram(program);
    glUniform3fv(objectColorUniform, 1, (float *)&objectColor[0]);
    glUniform3fv(lightColorUniform, 1, (float *)&lightColor[0]);
    glUniform3fv(lightPosUniform, 1, (float *)&lightTranslation[0]);
    glBindVertexArray(vao);
    glUniform1f(timeUniform, glfwGetTime());

    view = make_view(cameraPos, cameraPos + cameraDirection, up);

    glUniformMatrix4fv(viewUniform, 1, 0, (float *)&view[0][0]);
    glUniformMatrix4fv(projectionUniform, 1, 0, (float *)&projection[0][0]);

    for (int j = 0; j < translations_count; j++) {
      // angles[j] = step * i * (j + 1);
      model = make_model(translations[j], rotationVectors[j], angles[j]);

      glUniformMatrix4fv(modelUniform, 1, 0, (float *)&model[0][0]);

      glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  
	glfwTerminate();

	return 0;
}

float vectorXZAngle(vec3 vector) {
  return (vector.z > 0) ? acosf(vector.x) : -acosf(vector.x);
}

float vectorYZAngle(vec3 vector) {
  return (vector.y > 0) ? acosf(vector.z) : -acosf(vector.z);
}

vec3 rotateXZCamera(vec3 direction, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);
  vec3 output = vec3(c, direction.y, s);
  return output;
}

vec3 rotateYZCamera(vec3 direction, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);
  vec3 output = vec3(direction.x, s, c);
  return output;
}

float sign(float x) {
  if (x < 0.0) {
    return -1.0;
  } 
  else {
    return 1.0;
  }
}

void processInput(GLFWwindow *window) {

  float angle = M_PI / 128;
  float directionXZAngle = vectorXZAngle(cameraDirection);
  float directionYZAngle = vectorYZAngle(cameraDirection);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 d = normalize(cameraDirection);
    vec3 delta = d * 0.1f;
    cameraPos = cameraPos + delta;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 d = normalize(cameraDirection);
    vec3 delta = d * (-0.1f);
    cameraPos = cameraPos + delta;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos.x += 0.1;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos.x -= 0.1;
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    cameraPos.y += 0.1;
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    cameraPos.y -= 0.1;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    cameraDirection = rotateYZCamera(cameraDirection, directionYZAngle - angle);
    float oldCos = sign(cosf(directionYZAngle));
    float newCos = sign(cosf(directionYZAngle - angle));
    if (oldCos != newCos) {
      up = up * -1.0f;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    cameraDirection = rotateYZCamera(cameraDirection, directionYZAngle + angle);
    float oldCos = sign(cosf(directionYZAngle));
    float newCos = sign(cosf(directionYZAngle + angle));
    if (oldCos != newCos) {
      up = up * -1.0f;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    cameraDirection = rotateXZCamera(cameraDirection, directionXZAngle - angle);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    cameraDirection = rotateXZCamera(cameraDirection, directionXZAngle + angle);
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    cameraDirection = initialCameraDirection;
    cameraPos = vec3(0.0);
    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
  }
}
