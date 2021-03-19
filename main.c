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

int main( void )
{
	// Initialise GLFW
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( 1080, 1080, "Tutorial 02 - Red triangle", NULL, NULL);
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

  size_t points_size = 9 * sizeof(float);
  GLfloat up_triangle[] = {
    0, 0.5, 0,
    0.25, 0, 0,
    -0.25, 0, 0
	};
  
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

  double step = M_PI / 2048;
  GLfloat *triangle = (GLfloat *)malloc(points_size);
  for(int i = 0; should_close; i++) {
    memcpy(triangle, up_triangle, points_size);

    double angle1 = step * i;
    double angle2 = angle1 + (2 * M_PI / 3);
    double angle3 = angle2 + (2 * M_PI / 3);

    triangle[0] = cos(angle1);
    triangle[1] = sin(angle1);

    triangle[3] = cos(angle2);
    triangle[4] = sin(angle2);

    triangle[6] = cos(angle3);
    triangle[7] = sin(angle3);
        
    // double head_step = step * i;
    // double sin_value1 = sin(head_step);
    // double cos_value1 = cos(head_step);

    // double t = M_PI / 2;
    // double bottom_step2 = step * i - t;
    // double sin_value2 = -sin(bottom_step2);
    // double cos_value2 = -cos(bottom_step2);

    // double bottom_step3 = step * i + t;
    // double sin_value3 = sin(bottom_step3);
    // double cos_value3 = cos(bottom_step3);
    
    // memcpy(triangle, up_triangle, points_size);

    // triangle[0] = cos_value1;
    // triangle[1] = sin_value1;
    
    // triangle[4] = sin_value2 * 0.25;
    // triangle[3] *= cos_value2;
    
    // triangle[7] = sin_value3 * -0.25;
    // triangle[6] *= cos_value3;
    
    glBufferData(GL_ARRAY_BUFFER, points_size, triangle, GL_STATIC_DRAW);
    //    glClear(GL_COLOR_BUFFER_BIT);

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

