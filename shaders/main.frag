#version 330 core

#define PI 3.1415926538

uniform vec3 objectColor;

in vec3 fragPosition;

in vec3 LightColor;

out vec4 color;

void main() {
  vec3 result = LightColor * objectColor;
  color = vec4(result, 1.0);
}
