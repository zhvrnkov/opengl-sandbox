#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 outNormal;
out vec3 fragPosition;
out vec2 TexCoord;

void main() {
  mat3 modelWithoutTranslation = mat3(transpose(inverse(model)));
  
  float ar = 1080.0 / 1920.0;
  vec4 p = projection * view * model * vec4(position, 1);
  p.x = p.x * ar;
  gl_Position = p;
  outNormal = modelWithoutTranslation * normal;
  fragPosition = vec3(model * vec4(position, 1));
  TexCoord = aTexCoord;
}
