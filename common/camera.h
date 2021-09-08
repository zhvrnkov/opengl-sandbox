#ifndef camera_header
#define camera_header

#include <glm/glm.hpp>
using namespace glm;

mat4 make_view(vec3 position, vec3 direction, vec3 up);
mat4 make_projection(float w, float h, float n, float f);
mat4 make_projection_angle(float fovY, float aspectRatio, float n, float f);
mat4 make_model(vec3 translation, vec3 rotationVector, float rotationAngle);

#endif
