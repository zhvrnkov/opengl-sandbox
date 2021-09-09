#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 outNormal;
out vec3 fragPosition;

void main() {
  mat4 modelWithoutTranslation = model;
  modelWithoutTranslation[3].xyz = vec3(0.0);

  gl_Position = projection * view * model * vec4(position, 1);
  outNormal = vec3(modelWithoutTranslation * vec4(normal, 1));
  fragPosition = vec3(model * vec4(position, 1));
}
