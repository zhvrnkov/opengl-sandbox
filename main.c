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

#define should_close glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0

void panic_errno(const char *fmt, ...)
{
    fprintf(stderr, "ERROR: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errno));

    exit(1);
}

char *slurp_file(const char *file_path)
{
#define SLURP_FILE_PANIC panic_errno("Could not read file `%s`", file_path)
    FILE *f = fopen(file_path, "r");
    if (f == NULL) SLURP_FILE_PANIC;
    if (fseek(f, 0, SEEK_END) < 0) SLURP_FILE_PANIC;

    long size = ftell(f);
    if (size < 0) SLURP_FILE_PANIC;

    char *buffer = malloc(size + 1);
    if (buffer == NULL) SLURP_FILE_PANIC;

    if (fseek(f, 0, SEEK_SET) < 0) SLURP_FILE_PANIC;

    fread(buffer, 1, size, f);
    if (ferror(f) < 0) SLURP_FILE_PANIC;

    buffer[size] = '\0';

    if (fclose(f) < 0) SLURP_FILE_PANIC;

    return buffer;
#undef SLURP_FILE_PANIC
}

bool compile_shader_source(const GLchar *source, GLenum shader_type, GLuint *shader)
{
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLchar message[1024];
        GLsizei message_size = 0;
        glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
        fprintf(stderr, "%.*s\n", message_size, message);
        return false;
    }

    return true;
}

bool compile_shader_file(const char *file_path, GLenum shader_type, GLuint *shader)
{
    char *source = slurp_file(file_path);
    bool err = compile_shader_source(source, shader_type, shader);
    free(source);
    return err;
}

bool link_program(GLuint vert_shader, GLuint frag_shader, GLuint *program)
{
    *program = glCreateProgram();

    glAttachShader(*program, vert_shader);
    glAttachShader(*program, frag_shader);
    glLinkProgram(*program);

    GLint linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei message_size = 0;
        GLchar message[1024];

        glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
        fprintf(stderr, "Program Linking: %.*s\n", message_size, message);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

GLuint program = 0;
GLint resolution_location = 0;
GLint time_location = 0;

void reload_shaders(void) {
    glDeleteProgram(program);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint vert = 0;
    compile_shader_file("./main.vert", GL_VERTEX_SHADER, &vert);

    GLuint frag = 0;
    compile_shader_file("./main.frag", GL_FRAGMENT_SHADER, &frag);

    link_program(vert, frag, &program);

    glUseProgram(program);

    resolution_location = glGetUniformLocation(program, "resolution");
    time_location = glGetUniformLocation(program, "time");

    printf("Successfully Reload the Shaders\n");
}

#define COMPONENTS_COUNT 3
#define VECTOR_SIZE sizeof(float) * COMPONENTS_COUNT

void set_matrix(float **vectors, int vectors_count, float *output) {
  for (int vector_index = 0; vector_index < vectors_count; vector_index++) {
    for (int vector_component = 0; vector_component < COMPONENTS_COUNT; vector_component++) {
      int base = vector_index * COMPONENTS_COUNT;
      output[base + vector_component] = vectors[vector_index][vector_component];
    }
  }
}

float *make_matrix(float **vectors, int vectors_count) {
  int total_number_of_components = vectors_count * COMPONENTS_COUNT;
  float *output = malloc(total_number_of_components * sizeof(float));
  set_matrix(vectors, vectors_count, output);
  return output;
}

void set_sum(float *vec1, float *vec2, float *vec3, float *output) {
  for (int i = 0; i < COMPONENTS_COUNT; i++) {
    output[i] = vec1[i] + vec2[i] + vec3[i];
  }
}

float *sum(float *vec1, float *vec2, float *vec3) {
  float *output = malloc(VECTOR_SIZE);
  set_sum(vec1, vec2, vec3, output);
  return output;
}

void set_multiply(float *vector, float scalar, float *output) {
  for (int i = 0; i < COMPONENTS_COUNT; i++) {
    output[i] = vector[i] * scalar;
  }
}

float *multiply(float *vector, float scalar) {
  float *output = malloc(VECTOR_SIZE);
  set_multiply(vector, scalar, output);
  return output;
}

void set_transformed(float *vector, float **space, float *output) {
  float *transformed_x = multiply(space[0], vector[0]);
  float *transformed_y = multiply(space[1], vector[1]);
  float *transformed_z = multiply(space[2], vector[2]);

  set_sum(transformed_x, transformed_y, transformed_z, output);
  
  free(transformed_x);
  free(transformed_y);
  free(transformed_z);
}

float *transformed(float *vector, float **space) {
  float *transformed_x = multiply(space[0], vector[0]);
  float *transformed_y = multiply(space[1], vector[1]);
  float *transformed_z = multiply(space[2], vector[2]);

  float *output = sum(transformed_x, transformed_y, transformed_z);
  
  free(transformed_x);
  free(transformed_y);
  free(transformed_z);
  
  return output;
}

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

