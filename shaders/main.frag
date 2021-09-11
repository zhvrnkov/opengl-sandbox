#version 330 core

#define PI 3.1415926538

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 outNormal;
in vec3 fragPosition;

out vec4 color;

void main() {
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - fragPosition);

  float lightAmount = dot(norm, lightDir);
  float diff = max(lightAmount, 0.0);
  vec3 diffuse = diff * lightColor;

  float specularStrength = 1.0;
  vec3 reflection = reflect(-lightDir, norm);
  vec3 viewDir = normalize(viewPos - fragPosition);
  float spec = pow(max(dot(reflection, viewDir), 0.0), 64 );
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (diffuse + ambient + specular) * objectColor;
  color = vec4(result, 1.0);
}
