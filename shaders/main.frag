#version 330 core

#define PI 3.1415926538

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 outNormal;
in vec3 fragPosition;

out vec4 color;

void main() {
  vec3 ambient = material.ambient * light.ambient;

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - fragPosition);

  float lightAmount = dot(norm, lightDir);
  float diff = max(lightAmount, 0.0);
  vec3 diffuse = diff * material.diffuse * light.diffuse;

  vec3 reflection = reflect(-lightDir, norm);
  vec3 viewDir = normalize(viewPos - fragPosition);
  float spec = pow(max(dot(reflection, viewDir), 0.0), material.shininess);
  vec3 specular = material.specular * spec * light.specular;

  vec3 result = diffuse + specular + ambient;
  color = vec4(result, 1.0);
}
