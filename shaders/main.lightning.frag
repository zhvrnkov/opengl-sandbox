#version 330 core

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 color;
};

uniform Light light;

out vec4 FragColor;

void main()
{
    FragColor = vec4(light.color, 1.0);
}
