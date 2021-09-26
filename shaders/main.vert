#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 lightPos;

out vec3 outNormal;
out vec3 fragPosition;
out vec3 LightPos;

void main() {
  mat3 modelWithoutTranslation = mat3(transpose(inverse(view * model)));

  gl_Position = projection * view * model * vec4(position, 1);
  fragPosition = vec3(view * model * vec4(position, 1));
  outNormal = modelWithoutTranslation * normal;
  LightPos = vec3(view * vec4(lightPos, 1));
}
