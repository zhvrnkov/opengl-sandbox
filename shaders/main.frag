#version 330 core

#define PI 3.1415926538

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 outNormal;
in vec3 fragPosition;

out vec4 color;

void main() {
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - fragPosition);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (diffuse + ambient) * objectColor;
  color = vec4(result, 1.0);
}
