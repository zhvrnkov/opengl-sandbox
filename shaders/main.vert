#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 fragPosition;

out vec3 LightColor;

void main() {
  mat3 modelWithoutTranslation = mat3(transpose(inverse(model)));

  gl_Position = projection * view * model * vec4(position, 1);
  vec3 outNormal = modelWithoutTranslation * normal;
  fragPosition = vec3(model * vec4(position, 1));

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

  LightColor = (diffuse + ambient + specular);
}
