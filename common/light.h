#include <glm/glm.hpp>
using namespace glm;

typedef struct {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 color;
} Light;

void light_update(float time, Light *light) {
  light->color.x = sin(time * 2.0f);
  light->color.y = sin(time * 0.7f);
  light->color.z = sin(time * 1.3f);
  light->diffuse = light->color * 0.5f;
  light->ambient = light->color * 0.2f;
};
