#version 330 core

#define PI 3.1415926538

bool in_range(float value, float down, float up) {
  return value > down && value < up;
}

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct MapMaterial {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutoff;
  float outerCutoff;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform MapMaterial material;
uniform Light light;
uniform vec3 viewPos;
uniform float time;

in vec3 outNormal;
in vec3 fragPosition;
in vec2 TexCoord;

out vec4 color;

void main() {
  vec3 TexValue = vec3(texture(material.diffuse, TexCoord));
  vec3 ambient = TexValue * light.ambient;

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - fragPosition);
  float distance = length(light.position - fragPosition);
  float attenuation = 1.0 / (light.constant + light.linear * distance + 
                             light.quadratic * (distance * distance));

  float lightAmount = dot(norm, lightDir);
  float diff = max(lightAmount, 0.0);
  vec3 diffuse = diff * TexValue * light.diffuse;

  vec3 reflection = reflect(-lightDir, norm);
  vec3 viewDir = normalize(viewPos - fragPosition);
  float spec = pow(max(dot(reflection, viewDir), 0.0), material.shininess);
  vec3 specularTexture = vec3(texture(material.specular, TexCoord));
  vec3 specular = specularTexture * spec * light.specular;

  vec3 emission = vec3(0.0);
  if (in_range(TexCoord.x, 0.1, 0.9) && in_range(TexCoord.y, 0.1, 0.9)) {
    emission = vec3(texture(material.emission, (TexCoord * 0.8) + vec2(0.0, time)));
    emission *= (sin(time) / 2 + 1);
  }
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);


  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  ambient *= attenuation;
  vec3 result = diffuse + specular + ambient;
  color = vec4(result, 1.0);
}
